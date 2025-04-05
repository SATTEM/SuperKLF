#ifndef RELICEFFECT_H
#define RELICEFFECT_H
#include <string>
extern "C"{
	#include "raylib.h"
}
#include "nlohmann/json.hpp"
#include "Event/EventFWD.h"
#include "UI/UIUtility.h"
#include "ResourceManager.h"
class Entity;
class RelicEffect{
protected:
	std::wstring name=L"NULL",description;
	bool isActive=false;
	Texture2D icon;
public:
	RelicEffect()=default;
	RelicEffect(const nlohmann::json& params){
		name=UI::UTFTowstr(params["name"].get<std::string>());
		description=UI::UTFTowstr(params["description"].get<std::string>());
		std::string iconPath=ASSETS_IMAGE_PATH+params["icon_name"].get<std::string>();
		ResourceManager& resMgr=ResourceManager::Get();
		icon=resMgr.loadTexture(iconPath);
		icon=resMgr.resizeAndGet(iconPath,UI::RELIC_TEXTURE_SIZE);
	}
	virtual ~RelicEffect()=default;
	virtual void onTrigger(Entity& relatedEntity)=0;
	const std::wstring getDescription() const{return description;}
	const std::wstring getName()const{return name;}
	const Texture2D& getIcon()const{return icon;}
	virtual Occasion getOccasion() const=0;
};

class NullRelicEffect:public RelicEffect{
public:
	NullRelicEffect(const nlohmann::json& params):RelicEffect(){
		description=L"No effect";
		name=L"NULL";
	}
	void onTrigger(Entity& relatedEntity) override final{
		TraceLog(LOG_WARNING, "Applying a NullRelicEffect");
	}
	Occasion getOccasion() const override final{return Occasion::Undecided;}
	virtual ~NullRelicEffect()=default;
};

class DoubleShootRelic:public RelicEffect{
public:
	DoubleShootRelic(const nlohmann::json& params);
	void onTrigger(Entity& relatedEntity) override final;
	Occasion getOccasion() const override final{return Occasion::OnShoot;}
};


#endif