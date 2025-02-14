#ifndef BULLETEFFECT_H
#define BULLETEFFECT_H
#include "GameStageFwd.h"
class Entity;
class BulletEffect{
protected:
	Occasion triggerTime;
public:
	BulletEffect(const Occasion time=Occasion::OnHit):triggerTime(time){}
	virtual void tryTrigger(Entity& shooter,const Occasion& timing)=0;
};
class GiveDamage:public BulletEffect{
private:
	int damage;
public:
	GiveDamage(const int dmg):damage(dmg){triggerTime=Occasion::OnHit;}
	void tryTrigger(Entity& shooter,const Occasion& timing)override;
};
#endif