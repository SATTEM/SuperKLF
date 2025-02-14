#ifndef RELICEFFECT_H
#define RELICEFFECT_H
#include "Entity.h"
#include "GameStageFwd.h"
#include <string>

class RelicEffect{
protected:
	bool isActive=false;
	std::string description;
public:
	virtual ~RelicEffect()=default;
	virtual void onTrigger(const Occasion& timing,Entity& relatedEntity)=0;
	virtual std::string getDescription() const{return description;}
};

#endif