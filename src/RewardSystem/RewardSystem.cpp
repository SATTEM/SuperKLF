#include "RewardSystem.h"
#include "Effect/EffectManager.h"
#include "Effect/InstantEffect.h"
#include "Entity.h"
#include <memory>
#include <random>
#include <raylib.h>
#include <string>
#include "ResourceManager.h"
#include <cctype>
#include "UI/UIUtility.h"
std::wstring autoSplit(const std::wstring&);
bool isJsonValid(const nlohmann::json& j);
Reward::Reward(const nlohmann::json& reward){
	if(!isJsonValid(reward)){
		TraceLog(LOG_ERROR, "Reward json invalid!");
	}
	name=UI::UTFTowstr(reward["name"].get<std::string>());
	description=autoSplit(UI::UTFTowstr(reward["description"].get<std::string>()));
	icon=ResourceManager::Get().loadTexture(reward["icon_path"].get<std::string>());
	effect=EffectManager::Get().getInstantEffect(reward["effectID"].get<std::string>());
	if(name==L"新遗物"){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0,EffectManager::Get().getRelicEffectMap().size()-1);
		std::string relic_id;
		std::wstring relic_name=L"NULL";
		while(relic_name==L"NULL"){
			int randomIndex=dis(gen);
			relic_id=EffectManager::Get().getRelicEffectMap().at(randomIndex);
			std::dynamic_pointer_cast<NewRelic>(effect)->setRelicID(relic_id);
			relic_name=EffectManager::Get().getRelicEffect(relic_id)->getName();
		}
		description=autoSplit(description+L"\n"+relic_name);
		icon=ResourceManager::Get().loadTexture(ASSETS_IMAGE_PATH+relic_id+".png");
	}
}

std::wstring autoSplit(const std::wstring& str){
	std::wstring newStr;
	int charinline=0;
	for(const auto& c:str){
		newStr+=c;
		if(c=='\n'){
			charinline=0;
		}else{
			charinline++;
		}
		if(charinline>=UI::EXPLAIN_LINE_MAX_CHAR){
			newStr+='\n';
		}
	}
	return newStr;
}
void Reward::apply(Player& player){
	effect->onApply(player);
}

bool isJsonValid(const nlohmann::json& j){
	if(!j.contains("name")||!j.contains("description")||!j.contains("icon_path")||!j.contains("effectID")){
		return false;
	}
	return true;
}
