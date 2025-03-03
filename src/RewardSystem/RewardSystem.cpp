#include "RewardSystem.h"
#include "Effect/EffectManager.h"
#include "Entity.h"
void Reward::getEffect(const std::string& id){
	effect=EffectManager::Get().getInstantEffect(id);
}
void Reward::applyToEntity(Entity& entity){

}