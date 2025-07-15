#ifndef REWARDSYSTEM_H
#define REWARDSYSTEM_H

#include <memory>
#include <string>
#include "Effect/InstantEffect.h"
#include "Entity.h"
class Reward{
private:
	void toNewRelicReward(const std::string&);
	void toNewBulletReward(const std::string&);
	//转变奖励类型
	void adjust(const std::string&);
public:
	Reward(const nlohmann::json& reward);
	~Reward()=default;
	void apply(Player& player);
	const std::wstring& getDescription()const{return description;}
	const std::wstring& getName()const{return name;}
	const int& getCost()const{return cost;}
	const int& getRange()const{return range;}
private:
	std::wstring name,description;
	int cost=0;
	std::shared_ptr<InstantEffect> effect;	
	int range=0;
};
#endif