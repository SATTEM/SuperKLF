#include "DataManager.h"
#include "RewardSystem.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <string>
extern "C"{
	#include "raylib.h"
}

using json= nlohmann::json;
const int DataManager::getCoinsEarned() const{
	return DATA::BASIC_COINS_EARNED*DATA::BASIC_COINS_RATE*passedLevel;
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

const Reward& DataManager::getReward(const int i){
	return rewards.at(i);
}