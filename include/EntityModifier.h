#ifndef ENTITY_MODIFIER_H
#define ENTITY_MODIFIER_H
#include "Effect/RelicEffect.h"
#include "Entity.h"
#include <memory>
//提供修改Entity和Player的接口
class EntityModifier{
private:
	EntityModifier()=default;
public:
	EntityModifier(const EntityModifier&)=delete;
	EntityModifier& operator=(const EntityModifier&)=delete;
	static void addHP(Entity& entity,const int val);
	static void multiplyHP(Entity& entity,const float val);
	static void addMoney(Player& player,const int val);
	static void addRelic(Entity& entity,const std::shared_ptr<RelicEffect> relic);
	static void addAttackInterval(Entity& entity,const float val);
	static void multiplyAttackInterval(Entity& entity,const float rate);

};

#endif