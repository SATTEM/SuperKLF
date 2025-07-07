#include "Level/BattleLevel.h"
#include "Event/EventSystem.h"
#include "Effect/EffectManager.h"
#include "Level/Level.h"
#include "UI/LevelUI/BattleUI.h"
#include "DataManager.h"
#include "EntityModifier.h"
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
    static Vector2 enemyPos={
		GetScreenWidth()*UI::EntityCFG::DEFAULT_ENEMY_POSITION.x,
		GetScreenHeight()*UI::EntityCFG::DEFAULT_ENEMY_POSITION.y
	};
    Level::onActivate();
    std::unique_ptr<Enemy> tmp=std::make_unique<Enemy>(
        texPath,enemyPos,HP,attack_interval,
        max_energy,energy_rise
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
        getMoneyReward();
        requestLevelChange("Victory");
    }
}
void BattleLevel::getMoneyReward()const{
	//计算并给予奖励
	DataManager& dm=DataManager::Get();
	int coins=dm.getCoinsEarned();
	EntityModifier::addMoney(ctrl.getPlayer(), coins);
}