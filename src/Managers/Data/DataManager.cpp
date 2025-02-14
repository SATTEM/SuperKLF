#include "DataManager.h"
#include "ResourceManager.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <string>
extern "C"{
	#include "raylib.h"
}

using json= nlohmann::json;

std::string autoSplit(const std::string&);
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
			Reward reward;
			reward.name=item["name"].get<std::string>();
			reward.description=autoSplit(item["description"].get<std::string>());
			reward.icon=ResourceManager::Get().loadTexture(item["icon_path"].get<std::string>());
			for(const auto& effect:item["effects"]){
				reward.getEffects(effect);
			}
			rewards.push_back(std::move(reward));
		}
	}
}
std::string autoSplit(const std::string& str){
	std::string newStr;
	for(int i=0;i<str.length();i++){
		newStr+=str[i];
		if(i%26==0&&i!=0){
			newStr+='\n';
		}
	}
	newStr+='\0';
	return newStr;
}
const Reward& DataManager::getReward(const int i){
	return rewards.at(i);
}