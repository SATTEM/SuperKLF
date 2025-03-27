#include "RewardSystem.h"
#include "Effect/EffectManager.h"
#include "Effect/InstantEffect.h"
#include "Entity.h"
#include <memory>
#include <random>
#include <string>
#include "ResourceManager.h"
#include <cctype>
#include "UI/UIUtility.h"
std::string autoSplit(const std::string&);

Reward::Reward(const nlohmann::json& reward){
	name=reward["name"].get<std::string>();
	description=autoSplit(reward["description"].get<std::string>());
	icon=ResourceManager::Get().loadTexture(reward["icon_path"].get<std::string>());
	effect=EffectManager::Get().getInstantEffect(reward["effectID"].get<std::string>());
	if(name=="NewRelic"){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0,EffectManager::Get().getRelicEffectMap().size()-1);
		std::string relic_id,relic_name="NULL";
		while(relic_name=="NULL"){
			int randomIndex=dis(gen);
			relic_id=EffectManager::Get().getRelicEffectMap().at(randomIndex);
			std::dynamic_pointer_cast<NewRelic>(effect)->setRelicID(relic_id);
			relic_name=EffectManager::Get().getRelicEffect(relic_id)->getName();
		}
		description=autoSplit(reward["description"].get<std::string>()+"\n"+relic_name);
		icon=ResourceManager::Get().loadTexture(ASSETS_IMAGE_PATH+relic_id+".png");
	}
}

std::string autoSplit(const std::string& str){
	std::string newStr;
	int charinline=0;
	for(int i=0;i<str.length();i++){
		newStr+=str[i];
		if(str[i]=='\n'){
			charinline=0;
		}else{
			charinline++;
		}
		if(charinline>=UI::EXPLAIN_LINE_MAX_CHAR){
			newStr+='\n';
		}
	}
	newStr+='\0';
	return newStr;
}
void Reward::apply(Player& player){
	effect->onApply(player);
}


