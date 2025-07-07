#ifndef REWARDSYSTEM_H
#define REWARDSYSTEM_H

#include <memory>
#include <string>
#include "Effect/InstantEffect.h"
#include "Entity.h"
class Reward{
protected:
	std::wstring name,description;
	int cost=0;
	std::shared_ptr<InstantEffect> effect;
	void toNewRelicReward(const std::string&);
	void toNewBulletReward(const std::string&);
	void adjust(const std::string&);
public:
	Reward(const nlohmann::json& reward);
	~Reward()=default;
	void apply(Player& player);
	const std::wstring& getDescription()const{return description;}
	const std::wstring& getName()const{return name;}
	const int& getCost()const{return cost;}
};
#endif