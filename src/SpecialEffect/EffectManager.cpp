#include "Effect/EffectManager.h"
#include "Effect/InstantEffect.h"
#include "Effect/InstantEffectFactory.h"
#include "Effect/RelicEffect.h"
#include "Effect/RelicEffectFactory.h"
#include <memory>
#include <raylib.h>
using json = nlohmann::json;
//InstantEffectFactory的分派函数
void GenInstantEffectFactory(std::unique_ptr<InstantEffectFactory>& f,std::string id);
//RelicEffectFactory的分派函数
void GenRelicEffectFactory(std::unique_ptr<RelicEffectFactory>& f,std::string id);

void EffectManager::loadEffects(const json& json) const {
    try {
        if (json.contains("InstantEffects")) {
            InstantEffectLoader::Get().loadInstantEffect(json["InstantEffects"]);
        } else {
            TraceLog(LOG_WARNING, "No InstantEffects found in JSON");
        }

        if (json.contains("RelicEffects")) {
            RelicEffectLoader::Get().loadRelicEffect(json["RelicEffects"]);
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

std::shared_ptr<RelicEffect> EffectManager::getRelicEffect(const std::string& id) const {
    TraceLog(LOG_INFO, "Getting RelicEffect [id: %s]",id.c_str());
    return RelicEffectLoader::Get().getEffect(id);
}
void InstantEffectLoader::loadInstantEffect(const json& effects) {
    for (const auto& effect : effects) {
        try {
            std::string id = effect["id"].get<std::string>();
            std::unique_ptr<InstantEffectFactory> factory;
            GenInstantEffectFactory(factory, id);
            factory->loadFromJson(effect["params"]);
            factories[id] = std::move(factory);
            TraceLog(LOG_INFO, "Loaded InstantEffect: %s", id.c_str());
        } catch (const std::exception& e) {
            TraceLog(LOG_ERROR, "Failed to load InstantEffect: %s", e.what());
        }
    }
    if(factories.find(" ")==factories.end()){
        std::unique_ptr<InstantEffectFactory> f;
        GenInstantEffectFactory(f," ");
        factories[" "]=std::move(f);
    }
}
void GenInstantEffectFactory(std::unique_ptr<InstantEffectFactory>& f,std::string id){
    if(id=="speed_boost"){
        f=std::make_unique<ConcreteInstantEffectFactory<SpeedBoost>>();
    }else if(id=="health_boost"){
        f=std::make_unique<ConcreteInstantEffectFactory<HealthBoost>>();
    }else{
        f=std::make_unique<ConcreteInstantEffectFactory<NullInstantEffect>>();
    }
}
void RelicEffectLoader::loadRelicEffect(const json& relics) {
    std::string id, type;
    for (const auto& relic : relics) {
        try {
            id = relic["id"].get<std::string>();
            std::unique_ptr<RelicEffectFactory> factory;
            GenRelicEffectFactory(factory, id);
            factory->loadFromJson(relic["params"]);
            factories[id] = std::move(factory);
            TraceLog(LOG_INFO, "Loaded RelicEffect: %s", id.c_str());
        } catch (const std::exception& e) {
            TraceLog(LOG_ERROR, "Failed to load RelicEffect: %s", e.what());
        }
    }
    if(factories.find(" ")==factories.end()){
        std::unique_ptr<RelicEffectFactory> f;
        GenRelicEffectFactory(f," ");
        factories[" "]=std::move(f);
    }
}
void GenRelicEffectFactory(std::unique_ptr<RelicEffectFactory>& f,std::string id){
    if(id=="double_shoot_relic"){
       f=std::make_unique<ConcreteRelicFactory<DoubleShootRelic>>(); 
    }else{
       f=std::make_unique<ConcreteRelicFactory<NullRelicEffect>>(); 
    }
}
std::shared_ptr<InstantEffect> InstantEffectLoader::getEffect(const std::string& id) {
    if(factories.find(id)==factories.end()){
        TraceLog(LOG_ERROR,"Instant effect of %s not exist",id.c_str());
        return factories[" "]->create(); 
    }
    return factories[id]->create();
}

std::shared_ptr<RelicEffect> RelicEffectLoader::getEffect(const std::string& id) {
    if(factories.find(id)==factories.end()){
        TraceLog(LOG_ERROR,"Relic effect of %s not exist",id.c_str());
        return factories[" "]->create();
    }
    return factories[id]->create();
}