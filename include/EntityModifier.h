#ifndef ENTITY_MODIFIER_H
#define ENTITY_MODIFIER_H
#include "Effect/RelicEffect.h"
#include "Entity.h"
#include <algorithm>
#include <memory>
//提供修改Entity和Player的接口
class EntityModifier{
private:
	EntityModifier()=default;
public:
	EntityModifier(const EntityModifier&)=delete;
	EntityModifier& operator=(const EntityModifier&)=delete;
	static void addHP(Entity& entity,const int val){
		entity.maxHP+=val;
		entity.maxHP=std::max(1,entity.maxHP);
		entity.currentHP=entity.maxHP;
	}
	static void multiplyHP(Entity& entity,const float val){
		entity.maxHP=std::max(1.f,entity.maxHP*val);
		entity.currentHP=entity.maxHP;
	}
	static void addMoney(Player& player,const int val){
		player.money=std::max(0,player.money+val);
	}
	static void addRelic(Entity& entity,const std::shared_ptr<RelicEffect> relic){
		entity.addRelic(relic);
	}
	static void addAttackInterval(Entity& entity,const float val){
		entity.attackInterval=std::max(0.001f,entity.attackInterval+val);
	}
	static void multiplyAttackInterval(Entity& entity,const float rate){
		entity.attackInterval=std::max(0.001f,entity.attackInterval*rate);
	}

};

#endif