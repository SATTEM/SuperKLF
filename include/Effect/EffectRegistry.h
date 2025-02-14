#ifndef EFFECTREGISTRY_H
#define EFFECTREGISTRY_H
#include <memory>
#include <string>
#include <unordered_map>
#include "InstantEffect.h"
class EffectRegistry{
private:
	std::unordered_map<std::string,std::unique_ptr<InstantEffectFactory>> factories;
	EffectRegistry()=default;
	~EffectRegistry()=default;
public:
	static EffectRegistry& Get(){
		static EffectRegistry instance;
		return instance;
	}
	void registerFactory(const std::string& type,std::unique_ptr<InstantEffectFactory> factory){
		factories[type]=std::move(factory);
	}
	std::shared_ptr<InstantEffect> createInstantEffect(const std::string& type,const nlohmann::json& json){
		auto it=factories.find(type);
		if(it!=factories.end()){
			return it->second->createFromJson(json);
		}
		return nullptr;
	}
};
#endif