#ifndef INSTANTEFFECT_H
#define INSTANTEFFECT_H
#include <variant>
#include <memory>
#include "nlohmann/json.hpp"

class Player;
class InstantEffect{
public:
	InstantEffect()=default;
	virtual void trigger(Player& player)const =0;
};

class SpeedBoost:public InstantEffect{
private:
	float rate;
public:
	SpeedBoost(const float r):InstantEffect(){rate=r;}
	void trigger(Player& player) const override;
};

class HealthBoost:public InstantEffect{
private:
	std::variant<int,float> value;
public:
	HealthBoost(const float r):InstantEffect(){value=r;}
	HealthBoost(const int v):InstantEffect(){value=v;}
	void trigger(Player& player) const override;
};

class InstantEffectFactory{
public:
	virtual std::shared_ptr<InstantEffect> createFromJson(const nlohmann::json& json)=0;
	virtual ~InstantEffectFactory();
};
#endif