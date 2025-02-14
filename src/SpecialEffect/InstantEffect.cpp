#include "Effect/InstantEffect.h"
#include "Entity.h"
void SpeedBoost::trigger(Player& player) const{
	player.setAttackInterval(rate);
}

void HealthBoost::trigger(Player& player) const{
	const int* intPtr=std::get_if<int>(&value);
	if(intPtr!=nullptr){
		player.MaxHealthBoost(*intPtr);
	}else{
		player.MaxHealthBoost(*(std::get_if<float>(&value)));
	}
}