#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include "RewardSystem.h"
#include <string>
#include <vector>

namespace DATA {
	const int BASIC_COINS_EARNED=100;
	const float BASIC_COINS_RATE=0.15f;
	const std::string LEVEL_CONFIG_PATH=ASSETS_DATA_PATH "levels.json";
	const std::string REWARD_CONFIG_PATH=ASSETS_DATA_PATH "rewards.json";
	const std::string DATA_PATH=ASSETS_DATA_PATH "data.json";
	const std::string EFFECT_PATH=ASSETS_DATA_PATH "Effects.json";
}

class DataManager{
private:
	int passedLevel=0;
	float rewardRefreshRate=1.f;
	int rewardRefreshBase=10;
	int refreshTimes=0;
	std::vector<Reward> rewards;
	DataManager();
	~DataManager()=default;
public:
	DataManager(const DataManager&)=delete;
	void operator=(const DataManager&)=delete;
	static DataManager& Get(){
		static DataManager instance;
		return instance;
	}	
	void levelAdvance(){passedLevel+=1;}
	void refreshTimesAdvance(){refreshTimes+=1;}
	void setRewardRefreshBase(const int base){rewardRefreshBase=base;}
	void setRewardRefreshRate(const float rate){rewardRefreshRate=rate;}
	void resetRefreshTimes(){refreshTimes=0;}
	void reset(){passedLevel=0;rewardRefreshRate=1.f;rewardRefreshBase=10;refreshTimes=0;}
	const int getRefreshMoney() const{return refreshTimes*rewardRefreshBase*rewardRefreshRate;}
	const int getRefreshTimes() const{return refreshTimes;}
	const int getPassedLevel() const{return passedLevel;}
	const int getCoinsEarned() const;
	const Reward& getReward(const int i);
	const int getRewardSize() const {return rewards.size();}
};
#endif