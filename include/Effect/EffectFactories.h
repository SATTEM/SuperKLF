#ifndef EFFECTFACTORIES_H
#define EFFECTFACTORIES_H
#include "InstantEffect.h"
#include <memory>


class SpeedBoostFactory:public InstantEffectFactory{
public:
	std::shared_ptr<InstantEffect> createFromJson(const nlohmann::json& json) override{
		return std::make_shared<SpeedBoost>(json["rate"].get<float>());
	}
};
class HealthBoostFactory:public InstantEffectFactory{
public:
	std::shared_ptr<InstantEffect> createFromJson(const nlohmann::json& json) override{
		if(json["value"].is_number_integer()){
			return std::make_shared<HealthBoost>(json["value"].get<int>());
		}
		return std::make_shared<HealthBoost>(json["value"].get<float>());
	}
};

#endif