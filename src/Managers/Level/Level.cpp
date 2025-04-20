#include "Level.h"
#include "Bullet.h"
#include "DataManager.h"
#include "Effect/EffectManager.h"
#include "Entity.h"
#include "Event/EventSystem.h"
#include "GameStage.h"
#include "Tools.h"
#include "UI/UI.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <memory>
#include <raylib.h>
#include <string>
Level::Level(const nlohmann::json& json):ctrl(StageController::Get()){
    if(Check::isJsonValid(json,{"id"})){
        id=json["id"].get<std::string>();
    }
}
void Level::requestLevelChange(const std::string& nextLevelID){
    LevelManager::Get().switchToLevel(nextLevelID);
}
void MainMenuLevel::update(){
    MainMenuUI& ui=MainMenuUI::Get();
    ui.Draw();
    if(ui.isExit()){
        LevelManager::Get().setExitFlag();
    }else if(ui.isStart()){
        ctrl.resetGame();
        requestLevelChange("Victory");
    }
}

void DefeatLevel::update(){
    DefeatUI& ui=DefeatUI::Get();
    ui.Draw();
    if(ui.isRestart()){
        ctrl.resetGame();
        requestLevelChange("MainMenu");
    }else if(ui.isExit()){
        LevelManager::Get().setExitFlag();
    }
}
void VictoryLevel::update(){
    VictoryUI& ui=VictoryUI::Get();
    DataManager& data=DataManager::Get();
    if(data.getRefreshTimes()==0){
        ui.tryGenerateRewards(ctrl.getPlayer());
    }
    ui.Draw();
    if(ui.isRefreshButtonPressed()){
        ui.tryGenerateRewards(ctrl.getPlayer());
    }
    for(int i=0;i<3;i++){
        if(ui.isRewardButtonPressed(i)){
            ui.chooseReward(i, ctrl.getPlayer());
            data.levelAdvance();
            LevelManager::Get().toNextBattle();
        }
    }
}

BattleLevel::BattleLevel(const nlohmann::json& json):Level(json){
    bool notValid=!Check::isJsonValid(json,
    {"id","image","HP","bullets","relic_id",
    "max_energy","energy_rise","attack_interval",
    "blast","blast_image","name"});
    if(notValid){
        TraceLog(LOG_ERROR, "[DATA]: Invalid battle level data at: %s",id.c_str());
        return;
    }
    texPath=ASSETS_IMAGE_PATH+json["image"].get<std::string>();
    blast=json["blast"].get<std::string>();
    blastImage=ASSETS_IMAGE_PATH+json["blast_image"].get<std::string>();
    name=json["name"].get<std::string>();
    for(auto& bullet:json["bullets"]){
        bullets.push_back(bullet.get<std::string>());
    }
    for(auto& relic:json["relic_id"]){
        relics.push_back(relic.get<std::string>());
    }
    energy_rise=json["energy_rise"].get<int>();
    max_energy=json["max_energy"].get<int>();
    attack_interval=json["attack_interval"].get<float>();
    HP=json["HP"].get<int>();
}
void BattleLevel::onActivate(){
    Level::onActivate();
    std::unique_ptr<Enemy> tmp=std::make_unique<Enemy>(
        texPath,UI::DEFAULT_ENEMY_POSITION,
        HP,attack_interval,max_energy,energy_rise
    );
    for(const auto& bullet:bullets){
        tmp->addBulletByID(bullet);
    }
    if(!blast.empty()){
        Blast ablast=*BULLET::getBlast(blast);
        ablast.setImage(blastImage);
        tmp->setBlast(ablast);
    }
    for(const auto& relic:relics){
        if(EffectManager::Get().isRelicEffectLoaded(relic)){
            tmp->addRelic(EffectManager::Get().getRelicEffect(relic));
        }
    }
    ctrl.setEnemy(std::move(tmp));
    Player& player=ctrl.getPlayer();
    Enemy& enemy=ctrl.getEnemy();
	EventSystem::Get().broadcastEvent(Occasion::OnBattleStart,player);
	EventSystem::Get().broadcastEvent(Occasion::OnBattleStart, enemy);
	player.reset();
	enemy.reset();
	enemy.setOpponent(player);
	player.setOpponent(enemy);
}
void BattleLevel::update(){
    BattleUI& ui=BattleUI::Get();
    float deltaTime=GetFrameTime();
    Player& player=ctrl.getPlayer();
    Enemy& enemy=ctrl.getEnemy();
    player.Update(deltaTime);
    enemy.Update(deltaTime);
    ui.Draw();
    if(!player.isAlive()){
        requestLevelChange("Defeat");
    }else if(!enemy.isAlive()){
        requestLevelChange("Victory");
    }
}

EventLevel::EventLevel(const nlohmann::json& events):Level(events){

}

void EventLevel::update(){

}

LevelManager::LevelManager(){
}
void LevelManager::switchToLevel(const std::string& id){
    if(levels.find(id)==levels.end()){
        TraceLog(LOG_ERROR, "level not exist");
        setExitFlag();
        return;
    }
    currentLevel->onDeactivate();
    currentLevel=levels[id].get();
    currentLevel->onActivate();
}
void LevelManager::initialize(){
    levels["MainMenu"]=std::make_unique<MainMenuLevel>();
    levels["Victory"]=std::make_unique<VictoryLevel>();
    levels["Defeat"]=std::make_unique<DefeatLevel>();
    loadLevelFromJson();
    currentLevel=levels["MainMenu"].get();
    currentLevel->onActivate();
}
void LevelManager::toNextBattle(){
    //检查有无事件（概率检查+是否刚从事件回来），有则切入事件，无则前往下一关
    switchToLevel("Battle"+std::to_string(0));//just for test
}
void LevelManager::loadLevelFromJson(){
    if(FileExists(DATA::LEVEL_CONFIG_PATH.c_str())){
        try {
            std::ifstream file(DATA::LEVEL_CONFIG_PATH);
            nlohmann::json data = nlohmann::json::parse(file);
            
            if(!Check::isJsonValid(data, {"battles","events"})){
                TraceLog(LOG_ERROR, "level json not valid");
                return;
            }
            
            // 创建所有battle关卡
            int count=0;
            for(auto& battle : data["battles"]){
                // 跳过空对象
                if(battle.empty()) {
                    TraceLog(LOG_WARNING, "Skipping empty battle object in JSON");
                    continue;
                }
                
                // 验证必要字段
                if(!Check::isJsonValid(battle, {"id"})) {
                    TraceLog(LOG_ERROR, "Battle missing required 'id' field, skipping");
                    continue;
                }
                
                try {
                    std::unique_ptr<BattleLevel> tmp = std::make_unique<BattleLevel>(battle);
                    std::string id = tmp->getID();
                    if(id.empty()) {
                        TraceLog(LOG_ERROR, "Battle has empty ID, skipping");
                        continue;
                    }
                    levels[id] = std::unique_ptr<Level>(tmp.release());
                    count++;
                } catch(const std::exception& e) {
                    TraceLog(LOG_ERROR, "Failed to create battle level: %s", e.what());
                }
            }
            TraceLog(LOG_INFO, "[DATA]: Loaded %d battle levels", count);
            
            // 同样处理events
            count=0;
            for(auto& event : data["events"]){
                if(event.empty()) {
                    TraceLog(LOG_WARNING, "Skipping empty event object in JSON");
                    continue;
                }
                
                if(!Check::isJsonValid(event, {"id"})) {
                    TraceLog(LOG_ERROR, "Event missing required 'id' field, skipping");
                    continue;
                }
                
                try {
                    std::unique_ptr<EventLevel> tmp = std::make_unique<EventLevel>(event);
                    std::string id = tmp->getID();
                    if(id.empty()) {
                        TraceLog(LOG_ERROR, "Event has empty ID, skipping");
                        continue;
                    }
                    levels[id] = std::unique_ptr<Level>(tmp.release());
                    count++;
                } catch(const std::exception& e) {
                    TraceLog(LOG_ERROR, "Failed to create event level: %s", e.what());
                }
            }
            TraceLog(LOG_INFO, "[DATA]: Loaded %d events", count);
        } catch(const std::exception& e) {
            TraceLog(LOG_ERROR, "[DATA]: Exception while parsing level JSON: %s", e.what());
        }
    } else {
        TraceLog(LOG_ERROR, "[DATA]: level config not found: %s", DATA::LEVEL_CONFIG_PATH.c_str());
    }
}