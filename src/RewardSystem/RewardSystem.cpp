#include "RewardSystem.h"
#include "Effect/EffectManager.h"
#include "Effect/InstantEffect.h"
#include "Entity.h"
#include <memory>
#include <random>
#include <raylib.h>
#include <string>
#include <cctype>
#include "UI/UIUtility.h"
#include "Tools.h"

Reward::Reward(const nlohmann::json& reward){
	if(!Check::isJsonValid(reward,{"name","description","effectID"})){
		TraceLog(LOG_ERROR, "Reward json invalid!");
	}
	name=Trans::UTFTowstr(reward["name"].get<std::string>());
	description=UI::autoSplit(Trans::UTFTowstr(reward["description"].get<std::string>()),UI::ButtonCFG::EXPLAIN_LINE_MAX_CHAR);
	effect=EffectManager::Get().getInstantEffect(reward["effectID"].get<std::string>());
	if(reward.contains("id")){
		adjust(reward["id"].get<std::string>());
	}
}

void Reward::adjust(const std::string& id){
	if(id=="new_relic"){
		toNewRelicReward(id);
	}else if(id.find("new_bullet_")!=std::string::npos){
		toNewBulletReward(id);
	}
}

void Reward::toNewRelicReward(const std::string& id){
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
	description=UI::autoSplit(description+L"\n"+relic_name,UI::ButtonCFG::EXPLAIN_LINE_MAX_CHAR);
}

void Reward::toNewBulletReward(const std::string& id){
	std::wstring newBulletName=Trans::UTFTowstr(id.substr(11));
	description=UI::autoSplit(description+L"\n"+newBulletName,UI::ButtonCFG::EXPLAIN_LINE_MAX_CHAR);
}

void Reward::apply(Player& player){
	effect->onApply(player);
}
