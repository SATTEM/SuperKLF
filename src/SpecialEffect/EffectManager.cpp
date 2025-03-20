#include "Effect/EffectManager.h"
#include "Effect/InstantEffect.h"
#include "Effect/InstantEffectFactory.h"
#include "Effect/RelicEffect.h"
#include "Effect/RelicEffectFactory.h"
#include <memory>
#include <raylib.h>
#include <string>
using json = nlohmann::json;
//InstantEffectFactory的分派函数
void GenInstantEffectFactory(std::unique_ptr<InstantEffectFactory>& f,std::string id);
//RelicEffectFactory的分派函数
void GenRelicEffectFactory(std::unique_ptr<RelicEffectFactory>& f,std::string id);

bool EffectManager::isRelicEffectLoaded(const std::string& id) const{
    return RelicEffectLoader::Get().isEffectLoaded(id);
}
bool EffectManager::isInstantEffectLoaded(const std::string& id)const{
    return InstantEffectLoader::Get().isEffectLoaded(id);
}
void EffectManager::loadEffects(const json& json) {
    try {
        if (json.contains("InstantEffects")) {
            instantIDs=InstantEffectLoader::Get().loadInstantEffect(json["InstantEffects"]);
        } else {
            TraceLog(LOG_WARNING, "No InstantEffects found in JSON");
        }

        if (json.contains("RelicEffects")) {
            relicIDs=RelicEffectLoader::Get().loadRelicEffect(json["RelicEffects"]);
        } else {
            TraceLog(LOG_WARNING, "No RelicEffects found in JSON");
        }
    } catch (const std::exception& e) {
        TraceLog(LOG_ERROR, "Failed to load effects: %s", e.what());
    }
}

std::shared_ptr<InstantEffect> EffectManager::getInstantEffect(const std::string& id) const {
    TraceLog(LOG_INFO, "Getting InstantEffect [id: %s]",id.c_str());
    return InstantEffectLoader::Get().getEffect(id);
}
std::shared_ptr<InstantEffect> EffectManager::getInstantEffect(const int index)const{
    return getInstantEffect(instantIDs.at(index));
}
std::shared_ptr<RelicEffect> EffectManager::getRelicEffect(const std::string& id) const {
    TraceLog(LOG_INFO, "Getting RelicEffect [id: %s]",id.c_str());
    return RelicEffectLoader::Get().getEffect(id);
}
std::shared_ptr<RelicEffect> EffectManager::getRelicEffect(const int index)const{
    return getRelicEffect(relicIDs.at(index));
}
std::map<int,std::string> InstantEffectLoader::loadInstantEffect(const json& effects) {
    std::map<int,std::string> EffectIDs={{0," "}};
    int index=1;
    for (const auto& effect : effects) {
        try {
            std::string id = effect["id"].get<std::string>();
            std::unique_ptr<InstantEffectFactory> factory;
            GenInstantEffectFactory(factory, id);
            factory->loadFromJson(effect["params"]);
            factories[id] = std::move(factory);
            TraceLog(LOG_INFO, "Loaded InstantEffect: %s", id.c_str());
            EffectIDs[index]=id;
            index++;
        } catch (const std::exception& e) {
            TraceLog(LOG_ERROR, "Failed to load InstantEffect: %s", e.what());
        }
    }
    if(factories.find(" ")==factories.end()){
        std::unique_ptr<InstantEffectFactory> f;
        GenInstantEffectFactory(f," ");
        factories[" "]=std::move(f);
    }
    return EffectIDs;
}
void GenInstantEffectFactory(std::unique_ptr<InstantEffectFactory>& f,std::string id){
    if(id=="speed_boost"){
        f=std::make_unique<ConcreteInstantEffectFactory<SpeedBoost>>();
    }else if(id=="health_boost"){
        f=std::make_unique<ConcreteInstantEffectFactory<HealthBoost>>();
    }else if(id=="new_relic"){
        f=std::make_unique<ConcreteInstantEffectFactory<NewRelic>>();
    }
    else{
        f=std::make_unique<ConcreteInstantEffectFactory<NullInstantEffect>>();
    }
}

std::shared_ptr<InstantEffect> InstantEffectLoader::getEffect(const std::string& id) {
    if(factories.find(id)==factories.end()){
        TraceLog(LOG_ERROR,"Instant effect of %s not exist",id.c_str());
        return factories[" "]->create(); 
    }
    return factories[id]->create();
}

bool InstantEffectLoader::isEffectLoaded(const std::string& id)const{
    if(factories.find(id)!=factories.end()){return true;}
    return false;
}

std::map<int,std::string> RelicEffectLoader::loadRelicEffect(const json& relics) {
    std::map<int,std::string> EffectIDs={{0," "}};
    std::string id, type;
    int index=1;
    for (const auto& relic : relics) {
        try {
            id = relic["id"].get<std::string>();
            std::unique_ptr<RelicEffectFactory> factory;
            GenRelicEffectFactory(factory, id);
            factory->loadFromJson(relic["params"]);
            factories[id] = std::move(factory);
            TraceLog(LOG_INFO, "Loaded RelicEffect: %s", id.c_str());
            EffectIDs[index]=id;
            index++;
        } catch (const std::exception& e) {
            TraceLog(LOG_ERROR, "Failed to load RelicEffect: %s", e.what());
        }
    }
    if(factories.find(" ")==factories.end()){
        std::unique_ptr<RelicEffectFactory> f;
        GenRelicEffectFactory(f," ");
        factories[" "]=std::move(f);
    }
    return EffectIDs;
}
void GenRelicEffectFactory(std::unique_ptr<RelicEffectFactory>& f,std::string id){
    if(id=="double_shoot_relic"){
       f=std::make_unique<ConcreteRelicFactory<DoubleShootRelic>>(); 
    }
    else{
       f=std::make_unique<ConcreteRelicFactory<NullRelicEffect>>(); 
    }
}

bool RelicEffectLoader::isEffectLoaded(const std::string& id) const{
    if(factories.find(id)!=factories.end()){return true;}
    return false;
}


std::shared_ptr<RelicEffect> RelicEffectLoader::getEffect(const std::string& id) {
    if(factories.find(id)==factories.end()){
        TraceLog(LOG_ERROR,"Relic effect of %s not exist",id.c_str());
        return factories[" "]->create();
    }
    return factories[id]->create();
}