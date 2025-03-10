#ifndef RELICEFFECT_H
#define RELICEFFECT_H
#include <string>
extern "C"{
	#include "raylib.h"
}
#include "nlohmann/json.hpp"
class Entity;
class RelicEffect{
protected:
	bool isActive=false;
	std::string description;
public:
	RelicEffect()=default;
	virtual ~RelicEffect()=default;
	virtual void onTrigger(Entity& relatedEntity)=0;
	virtual std::string getDescription() const{return description;}
};
class DoubleShootRelic:public RelicEffect{
public:
	DoubleShootRelic(const nlohmann::json& params):RelicEffect(){
		description=params["description"].get<std::string>();
	}
	void onTrigger(Entity& relatedEntity) override final{
		TraceLog(LOG_WARNING,"Triggering DoubleShootRelic(Not developed yet)");
	}
};
class NullRelicEffect:public RelicEffect{
public:
	NullRelicEffect(const nlohmann::json& params):RelicEffect(){
		description="No effect";
	}
	void onTrigger(Entity& relatedEntity) override final{
		TraceLog(LOG_WARNING, "Applying a NullRelicEffect");
	}
	virtual ~NullRelicEffect()=default;
};

#endif