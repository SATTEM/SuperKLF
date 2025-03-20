#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H
#include "InstantEffect.h"
#include "RelicEffect.h"
#include "nlohmann/json.hpp"
#include "InstantEffectFactory.h"
#include "RelicEffectFactory.h"
#include <memory>
#include <string>
#include <map>
#include <unordered_map>

class EffectManager{
private:
	EffectManager()=default;
	~EffectManager()=default;
	std::map<int,std::string> instantIDs;
	std::map<int,std::string> relicIDs;
public:
	EffectManager(const EffectManager&)=delete;
	void operator=(const EffectManager&)=delete;
	static EffectManager& Get(){
		static EffectManager instance;
		return instance;
	}
	//分发加载任务并获得所有效果id的集合
	void loadEffects(const nlohmann::json&);
	//从InstantEffectLoader获取一个即时效果
	std::shared_ptr<InstantEffect> getInstantEffect(const std::string&)const;
	std::shared_ptr<InstantEffect> getInstantEffect(const int index)const;
	//Relic效果同理
	std::shared_ptr<RelicEffect> getRelicEffect(const std::string&) const;
	std::shared_ptr<RelicEffect> getRelicEffect(const int index)const;
	//检查是否已经加载了效果
	bool isInstantEffectLoaded(const std::string& id) const;
	bool isRelicEffectLoaded(const std::string& id) const;
	const std::map<int,std::string>& getInstantEffectMap()const{return instantIDs;}
	const std::map<int,std::string>& getRelicEffectMap()const{return relicIDs;}
};
class InstantEffectLoader{
private:
	InstantEffectLoader()=default;
	~InstantEffectLoader()=default;
	std::unordered_map<std::string, std::unique_ptr<InstantEffectFactory>> factories;
public:
	InstantEffectLoader(const InstantEffectLoader&)=delete;
	void operator=(const InstantEffectLoader&)=delete;
	static InstantEffectLoader& Get(){
		static InstantEffectLoader instance;
		return instance;
	}
	//加载json中即时效果数组的每一个元素的工厂
	std::map<int,std::string> loadInstantEffect(const nlohmann::json& effects);
	//通过已加载的效果工厂创建效果
	std::shared_ptr<InstantEffect> getEffect(const std::string&);
	//检查是否存在某效果
	bool isEffectLoaded(const std::string& id) const;
};
//RelicEffect
class RelicEffectLoader{
private:
	RelicEffectLoader()=default;
	~RelicEffectLoader()=default;
	std::unordered_map<std::string, std::shared_ptr<RelicEffectFactory>> factories;
public:
	RelicEffectLoader(const RelicEffectLoader&)=delete;
	void operator=(const RelicEffectLoader&)=delete;
	static RelicEffectLoader& Get(){
		static RelicEffectLoader instance;
		return instance;
	}
	//加载json中遗物效果数组的每一个元素
	std::map<int,std::string> loadRelicEffect(const nlohmann::json& json);
	//通过已加载的效果工厂创建效果
	std::shared_ptr<RelicEffect> getEffect(const std::string&);
	//检查是否存在某效果
	bool isEffectLoaded(const std::string& id)const;
};
#endif