#include "Effect/InstantEffect.h"
#include "Effect/EffectManager.h"
#include "EntityModifier.h"
#include "Entity.h"
#include <string>
using EM=EntityModifier;
SpeedBoost::SpeedBoost(const nlohmann::json& params):InstantEffect(){
	rate=params["rate"].get<float>();
}
void SpeedBoost::onApply(Player& player)const{
	EM::multiplyAttackInterval(player, rate);
}

HealthBoost::HealthBoost(const nlohmann::json& params):InstantEffect(){
	if(params["value"].is_number_float()){
		value=params["value"].get<float>();
	}else{
		value=params["value"].get<int>();
	}
}
void HealthBoost::onApply(Player& player)const{
	const int* intPtr=std::get_if<int>(&value);
	if(intPtr!=nullptr){
		EM::addHP(player,*intPtr);
	}else{
		EM::multiplyHP(player,*(std::get_if<float>(&value)));
	}
}

NewRelic::NewRelic(const nlohmann::json& params):InstantEffect(){
	relic_id="null_relic_effect";
}
void NewRelic::onApply(Player& player)const{
	if(relic_id=="null_relic_effect"){
		TraceLog(LOG_ERROR,"creating a new relic with default id!");
		return;
	}else{
		if(EffectManager::Get().isRelicEffectLoaded(relic_id)){
			EM::addRelic(player,EffectManager::Get().getRelicEffect(relic_id));
		}else{
			TraceLog(LOG_ERROR, "creating a new relic with unknown id: %s",relic_id.c_str());
		}
	}
}
void NewRelic::setRelicID(const std::string& id){
	relic_id=id;
}