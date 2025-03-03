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
	SpeedBoost(const nlohmann::json& params):InstantEffect(){rate=params["rate"].get<float>();}
	void onApply(Player& player) const override final{
		player.setAttackInterval(rate);
	}
	virtual ~SpeedBoost() override=default;
};

class HealthBoost:public InstantEffect{
private:
	std::variant<int,float> value;
public:
	HealthBoost(const nlohmann::json& params):InstantEffect(){
		if(params["value"].is_number_float()){
			value=params["value"].get<float>();
		}else{
			value=params["value"].get<int>();
		}
	}
	void onApply(Player& player) const override final{
		const int* intPtr=std::get_if<int>(&value);
		if(intPtr!=nullptr){
			player.MaxHealthBoost(*intPtr);
		}else{
			player.MaxHealthBoost(*(std::get_if<float>(&value)));
		}
	}
	virtual ~HealthBoost()=default;
};

#endif