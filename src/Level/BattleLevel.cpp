#include "Level/BattleLevel.h"
#include "Event/EventSystem.h"
#include "Effect/EffectManager.h"
#include "Level/Level.h"
#include "UI/LevelUI/BattleUI.h"
#include "DataManager.h"
#include "EntityModifier.h"
const int adjustHP(const int origin,const int level,const int range);
BattleLevel::BattleLevel(const nlohmann::json& json):Level(json){
    //检查json是否合法
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
    if(json.contains("range")){
        range=json["range"].get<int>();
    }
}
void BattleLevel::onActivate(){
    static Vector2 enemyPos={
		GetScreenWidth()*UI::EntityCFG::DEFAULT_ENEMY_POSITION.x,
		GetScreenHeight()*UI::EntityCFG::DEFAULT_ENEMY_POSITION.y
	};
    Level::onActivate();
    int realHP=adjustHP(HP, DataManager::Get().getPassedLevel()-1,range);
    std::unique_ptr<Enemy> tmp=std::make_unique<Enemy>(
        texPath,enemyPos,realHP,attack_interval,
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
const int adjustHP(const int origin,const int level,const int range){
    /*敌人HP公式：
    range<=level<range+2: return origin
    range+2<=level<range+5: return origin*=DATA::ENEMY_BOOST_1st
    range+5<=level<range+7: return origin*=DATA::ENEMY_BOOST_2nd
    range+7<=level<range+9: return origin*=DATA::ENEMY_BOOST_3rd
    level>=range+9: return origin*=DATA::ENEMY_BOOST_4th
    */
    int realHP=origin;
    if(range+2<=level&&level<range+5){
        realHP*=DATA::EnemyCFG::ENEMY_BOOST_1st;
    }else if(range+5<=level&&level<range+7){
        realHP*=DATA::EnemyCFG::ENEMY_BOOST_2nd;
    }else if(range+7<=level&&level<range+9){
        realHP*=DATA::EnemyCFG::ENEMY_BOOST_3rd;
    }else if(level>=range+9){
        realHP*=DATA::EnemyCFG::ENEMY_BOOST_4th;
    }
    return realHP;
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