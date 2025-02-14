#include "Effect/BulletEffect.h"
#include "Entity.h"
void GiveDamage::tryTrigger(Entity& shooter,const Occasion& timing){
	if(timing==Occasion::OnHit){
		shooter.getOpponent().takeDamage(damage);
	}
}