#include "EntityModifier.h"
#include <algorithm>
void EntityModifier::addHP(Entity& entity,const int val){
	entity.maxHP+=val;
	entity.maxHP=std::max(1,entity.maxHP);
	entity.currentHP=entity.maxHP;
}
void EntityModifier::multiplyHP(Entity& entity,const float val){
	entity.maxHP=std::max(1.f,entity.maxHP*val);
	entity.currentHP=entity.maxHP;
}
void EntityModifier::addRelic(Entity& entity,const std::shared_ptr<RelicEffect> relic){
	entity.addRelic(relic);
}
void EntityModifier::addAttackInterval(Entity& entity,const float val){
	entity.attackInterval=std::max(0.001f,entity.attackInterval+val);
}
void EntityModifier::multiplyAttackInterval(Entity& entity,const float rate){
	entity.attackInterval=std::max(0.001f,entity.attackInterval*rate);
}
void EntityModifier::addMoney(Player& player,const int val){
	player.money=std::max(0,player.money+val);
}
