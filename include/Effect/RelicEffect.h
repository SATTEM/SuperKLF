#ifndef RELICEFFECT_H
#define RELICEFFECT_H
#include <string>
extern "C"{
	#include "raylib.h"
}
#include "nlohmann/json.hpp"
#include "Event/EventFWD.h"
class Entity;
class RelicEffect{
protected:
	std::string name;
	bool isActive=false;
	std::string description;
public:
	RelicEffect()=default;
	RelicEffect(const nlohmann::json& params){
		name=params["name"].get<std::string>();
		description=params["name"].get<std::string>();
	}
	virtual ~RelicEffect()=default;
	virtual void onTrigger(Entity& relatedEntity)=0;
	const std::string getDescription() const{return description;}
	const std::string getName()const{return name;}
	virtual Occasion getOccasion() const=0;
};

class NullRelicEffect:public RelicEffect{
public:
	NullRelicEffect(const nlohmann::json& params):RelicEffect(){
		description="No effect";
		name="NULL";
	}
	void onTrigger(Entity& relatedEntity) override final{
		TraceLog(LOG_WARNING, "Applying a NullRelicEffect");
	}
	Occasion getOccasion() const override final{return Occasion::Undecided;}
	virtual ~NullRelicEffect()=default;
};

class DoubleShootRelic:public RelicEffect{
public:
	DoubleShootRelic(const nlohmann::json& params);
	void onTrigger(Entity& relatedEntity) override final;
	Occasion getOccasion() const override final{return Occasion::OnShoot;}
};


#endif