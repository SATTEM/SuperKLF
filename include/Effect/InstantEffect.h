#ifndef INSTANTEFFECT_H
#define INSTANTEFFECT_H
#include <raylib.h>
#include <variant>
#include "nlohmann/json.hpp"
#include "Entity.h"
class InstantEffect{
public:
	InstantEffect()=default;
	virtual void onApply(Player& player)const =0;
	virtual ~InstantEffect()=default;
};
class NullInstantEffect:public InstantEffect{
public:
	NullInstantEffect(const nlohmann::json& params):InstantEffect(){}
	void onApply(Player& player) const override final{
		TraceLog(LOG_WARNING, "Applying a NullInstantEffect");
	}
	virtual ~NullInstantEffect(){}
};

class SpeedBoost:public InstantEffect{
private:
	float rate;
public:
	SpeedBoost(const nlohmann::json& params);
	void onApply(Player& player) const override final;
	virtual ~SpeedBoost() override=default;
};

class HealthBoost:public InstantEffect{
private:
	std::variant<int,float> value;
public:
	HealthBoost(const nlohmann::json& params);
	void onApply(Player& player) const override final;
	virtual ~HealthBoost()=default;
};

class NewRelic:public InstantEffect{
private:
	std::string relic_id;
public:
	NewRelic(const nlohmann::json& params);
	void onApply(Player& player) const override final;
	void setRelicID(const std::string& id);
	virtual ~NewRelic()=default;
};

#endif