#include "Level/LevelManager.h"
#include "GameStage.h"
#include "Level/Level.h"
#include "Level/MainMenuLevel.h"
#include "Level/DefeatLevel.h"
#include "Level/ShopLevel.h"
#include "Level/VictoryLevel.h"
#include "Level/EventLevel.h"
#include "Level/BattleLevel.h"
#include "DataManager.h"
#include <fstream>
#include <random>
#include <raylib.h>
LevelManager::LevelManager(){
}
void LevelManager::switchToLevel(const std::string& id){
    if(levels.find(id)==levels.end()){
        TraceLog(LOG_ERROR, "Level not exist: %s",id.c_str());
        setExitFlag();
        return;
    }
    currentLevel->onDeactivate();
    TraceLog(LOG_INFO, "Going to level: %s",id.c_str());
    currentLevel=levels[id].get();
    currentLevel->onActivate();
}
void LevelManager::initialize(){
    levels["MainMenu"]=std::make_unique<MainMenuLevel>();
    levels["Victory"]=std::make_unique<VictoryLevel>();
    levels["Defeat"]=std::make_unique<DefeatLevel>();
    levels["Shop"]=std::make_unique<ShopLevel>();
    loadLevelFromJson();
    currentLevel=levels["MainMenu"].get();
    currentLevel->onActivate();
}
void LevelManager::toNextBattle(){
    if(shouldEnterEvent()){
        //应该进入事件
        switchToLevel(generateEventID());
    }else if(shouldEnterShop()){
        //应该进入商店
        switchToLevel("Shop");
    }else{
        //应该进入战斗：先存档再战斗
        StageController::Get().captureGame();
        switchToLevel(generateBattleID());
    }
}
const bool LevelManager::shouldEnterShop()const{
    if(dynamic_cast<ShopLevel*>(currentLevel)!=nullptr||dynamic_cast<EventLevel*>(currentLevel)!=nullptr){
        //刚从商店或事件回来
        return false;
    }
    //或不满足进入商店的条件
    if(StageController::Get().getPlayer().getMoney()<DATA::SHOP_MONEY_LIMIT||DataManager::Get().getPassedLevel()==1){
        return false;
    }

    return true;
}
const bool LevelManager::shouldEnterEvent()const{
    if(dynamic_cast<EventLevel*>(currentLevel)!=nullptr||DataManager::Get().getPassedLevel()==1){
        //刚从事件回来
        //或刚开始游戏（此时passedLevel会是1而不是0）
        return false;
    }
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> dis(0.f,1.f);
    float num=dis(engine);
    if(num<=DataManager::Get().getEventOdds()){
        return true;
    }else{
        return false;
    }
}
const std::string LevelManager::generateEventID()const{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dis(0,eventCount-1);
    std::string eventID="Event"+std::to_string(dis(engine));
    EventLevel* event=dynamic_cast<EventLevel*>(levels.at(eventID).get());
    int seekCount=0;//最多寻找次数
    while(event->getRange()>DataManager::Get().getPassedLevel()&&seekCount<50){
        //若未达到出现关卡
        seekCount++;
        eventID="Battle"+std::to_string(dis(engine));
        event=dynamic_cast<EventLevel*>(levels.at(eventID).get());
    }
    if(seekCount>=50){
        TraceLog(LOG_ERROR, "Not find usable event level after 50 tries");
    }
    return eventID;
}
const std::string LevelManager::generateBattleID()const{
    //获得下一场战斗的ID
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dis(0,battleCount-1);
    std::string battleID="Battle"+std::to_string(dis(engine));
    BattleLevel* battle=dynamic_cast<BattleLevel*>(levels.at(battleID).get());
    int seekCount=0;//最多寻找次数
    while(battle->getRange()>DataManager::Get().getPassedLevel()&&seekCount<50){
        //若未达到出现关卡
        seekCount++;
        battleID="Battle"+std::to_string(dis(engine));
        battle=dynamic_cast<BattleLevel*>(levels.at(battleID).get());
    }
    if(seekCount>=50){
        TraceLog(LOG_ERROR, "Not find usable battle level after 50 tries");
    }
    return battleID;
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
            battleCount=count;
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
            eventCount=count;
        } catch(const std::exception& e) {
            TraceLog(LOG_ERROR, "[DATA]: Exception while parsing level JSON: %s", e.what());
        }
    } else {
        TraceLog(LOG_ERROR, "[DATA]: level config not found: %s", DATA::LEVEL_CONFIG_PATH.c_str());
    }
}