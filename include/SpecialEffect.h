#ifndef SPECIALEFFECT_H
#define SPECIALEFFECT_H
#include "Entity.h"
#include "GameStageFwd.h"

class BulletEffectFactory{

};

class BulletEffect{
protected:
	Occasion triggerTime;
public:
	BulletEffect(const Occasion time=Occasion::OnHit):triggerTime(time){}
	virtual void tryTrigger(Entity& shooter,const Occasion& timing)=0;
	const Occasion when() const{return triggerTime;}
};
class GiveDamage:public BulletEffect{
private:
	int damage;
public:
	GiveDamage(int dmg):damage(dmg){triggerTime=Occasion::OnHit;}
	void tryTrigger(Entity& shooter,const Occasion& timing) override;
	void setDamage(const int dmg){damage=dmg;}
};



#endif