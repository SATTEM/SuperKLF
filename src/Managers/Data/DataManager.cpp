#include "DataManager.h"
#include "RewardSystem.h"
#include "nlohmann/json.hpp"
#include <algorithm>
#include <fstream>
#include <string>
extern "C"{
	#include "raylib.h"
}

using json= nlohmann::json;
void DataManager::reset(){
	passedLevel=0;
	rewardRefreshRate=1.f;
	rewardRefreshBase=10;
	refreshTimes=0;
	eventOdds=DATA::BASIC_EVENT_ODDS;
}
const int DataManager::getCoinsEarned() const{
	return DATA::BASIC_COINS_EARNED*DATA::BASIC_COINS_RATE*passedLevel;
}
void DataManager::setEventOdds(const char& op,const float val){
	//op:'+' '-' '*' '/'
	//'s' for set to
	switch(op){
		case('+'):
		eventOdds+=val;
		break;
		case('-'):
		eventOdds-=val;
		break;
		case('*'):
		eventOdds*=val;
		break;
		case('/'):
		if(val==0){eventOdds=1;}
		else{eventOdds/=val;}
		break;
		case('s'):
		eventOdds=val;
		break;
	}
	eventOdds=std::clamp(eventOdds,0.f,1.f);
}
DataManager::DataManager(){
	//用nlohmann json从data.json中读取存档信息等
	//从rewards.json读取可用奖励
	if(FileExists(DATA::REWARD_CONFIG_PATH.c_str())){
		std::ifstream file{DATA::REWARD_CONFIG_PATH};
		json data=json::parse(file);
		rewards.reserve(data.size());
		for(auto& item :data){
			rewards.push_back({item});
			TraceLog(LOG_INFO, "[RES] Reward Loaded: %s",item["name"].get<std::string>().c_str());
		}
	}
}
const int DataManager::getRefreshMoney()const{
	int num=refreshTimes*rewardRefreshBase*rewardRefreshRate;
	if(num>=DATA::BASIC_MAX_REFRESH_COIN){num=DATA::BASIC_MAX_REFRESH_COIN;}
	return num;
}
const Reward& DataManager::getReward(const int i){
	return rewards.at(i);
}