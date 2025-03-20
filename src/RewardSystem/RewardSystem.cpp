#include "RewardSystem.h"
#include "Effect/EffectManager.h"
#include "Effect/InstantEffect.h"
#include "Entity.h"
#include <memory>
#include <random>
#include <string>
#include "ResourceManager.h"
#include <cctype>

std::string autoSplit(const std::string&);
std::string transRelicIDToName(const std::string&);

Reward::Reward(const nlohmann::json& reward){
	name=reward["name"].get<std::string>();
	description=autoSplit(reward["description"].get<std::string>());
	icon=ResourceManager::Get().loadTexture(reward["icon_path"].get<std::string>());
	effect=EffectManager::Get().getInstantEffect(reward["effectID"].get<std::string>());
	if(name=="NewRelic"){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0,EffectManager::Get().getRelicEffectMap().size()-1);
		int randomIndex=dis(gen);
		std::string relic_id=EffectManager::Get().getRelicEffectMap().at(randomIndex);
		std::dynamic_pointer_cast<NewRelic>(effect)->setRelicID(relic_id);
		std::string relic_name=transRelicIDToName(relic_id);
		name="Relic: "+relic_name;
		description=autoSplit(reward["description"].get<std::string>()+relic_name);
		icon=ResourceManager::Get().loadTexture(ASSETS_IMAGE_PATH+relic_id+".png");
	}
}

std::string autoSplit(const std::string& str){
	std::string newStr;
	for(int i=0;i<str.length();i++){
		newStr+=str[i];
		if(i%26==0&&i!=0&&str[i]!='\n'){
			newStr+='\n';
		}
	}
	newStr+='\0';
	return newStr;
}
void Reward::apply(Player& player){
	effect->onApply(player);
}

std::string transRelicIDToName(const std::string& id){
	std::string name=id;
	for(auto it=name.begin();it!=name.end();it++){
		if(*(it-1)=='_'||it==name.begin()){
			*it=std::toupper(*it);
		}
		if(*it=='_'){
			it=name.erase(it);
		}
	}
	return name;
}

