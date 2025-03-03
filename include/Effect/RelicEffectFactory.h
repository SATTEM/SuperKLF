#ifndef RELICEFFECTFACTORY_H
#define RELICEFFECTFACTORY_H
#include "RelicEffect.h"
#include "nlohmann/json.hpp"
#include <memory>
class RelicEffectFactory{
public:
	virtual std::shared_ptr<RelicEffect> create()=0;
	virtual void loadFromJson(const nlohmann::json&)=0;
};
template<typename T>
class ConcreteRelicFactory:public RelicEffectFactory{
private:
	nlohmann::json params;
public:
	std::shared_ptr<RelicEffect> create() override{
		return std::make_unique<T>(params);
	}
	void loadFromJson(const nlohmann::json& relic) override{
		params=relic;
	}
};
#endif