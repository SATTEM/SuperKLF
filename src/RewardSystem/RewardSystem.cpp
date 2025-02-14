#include "RewardSystem.h"
#include "Effect/EffectRegistry.h"
extern "C"{
	#include "raylib.h"	
}

using json=nlohmann::json;

void Reward::getEffects(const json& j){
	std::string type=j["type"].get<std::string>();
	if(auto effect=EffectRegistry::Get().createInstantEffect(type, j)){
		effects.push_back(effect);
	}else{
		TraceLog(LOG_WARNING,"Unknown instant effect type: %s",type.c_str());
	}
}