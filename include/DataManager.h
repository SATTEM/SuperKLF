#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include "RewardSystem.h"
#include <string>
#include <vector>

namespace DATA {
	//游戏初始数值
	const int BASIC_COINS_EARNED=100;
	const int BASIC_MAX_REFRESH_COIN=100;
	const float BASIC_COINS_RATE=0.15f;
	const float BASIC_EVENT_ODDS=0.5f;
	const int SKIP_MONEY=20;
	const int SHOP_GOODS_SIZE=5;
	const int SHOP_MONEY_LIMIT=100;
	namespace EnemyCFG{
		//Enemy相关数值
		const float ENEMY_BOOST_1st=1.2;
		const float ENEMY_BOOST_2nd=1.7;
		const float ENEMY_BOOST_3rd=2.5;
		const float ENEMY_BOOST_4th=4;		
	}
	//Player相关数值
	const int START_MONEY=9999999;
	const int PLAYER_HP=100;
	const float PLAYER_INTERVAL=0.5f;
	const int PLAYER_ENERGY=100;
	const int PLAYER_RISE=10;
	//路径设置
	const std::string LEVEL_CONFIG_PATH=ASSETS_DATA_PATH "Levels.json";
	const std::string REWARD_CONFIG_PATH=ASSETS_DATA_PATH "Rewards.json";
	const std::string DATA_PATH=ASSETS_DATA_PATH "Data.json";
	const std::string EFFECT_PATH=ASSETS_DATA_PATH "Effects.json";
}

class DataManager{
private:
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
	void setEventOdds(const char&,const float);
	void resetRefreshTimes(){refreshTimes=0;}
	void reset();
	const int getRefreshMoney() const;
	const int getRefreshTimes() const{return refreshTimes;}
	const int getPassedLevel() const{return passedLevel;}
	const int getCoinsEarned() const;
	const float getEventOdds()const{return eventOdds;}
	const int getShopGoodSize()const{return shopGoodSize;}
	const Reward& getReward(const int i);
	const Reward& getRandomReward(const bool isFilter=false);
	const int getRewardSize() const {return rewards.size();}
private:
	int passedLevel=0;
	float rewardRefreshRate=1.f;
	int rewardRefreshBase=10;
	int refreshTimes=0;
	int shopGoodSize=DATA::SHOP_GOODS_SIZE;
	std::vector<Reward> rewards;
	float eventOdds=DATA::BASIC_EVENT_ODDS;	
};
#endif