#include "Level/EventLevel.h"
#include "Level/LevelManager.h"
#include "UI/LevelUI/EventUI.h"
#include "Effect/EffectManager.h"
#include <raylib.h>
EventLevel::Option::Option(const nlohmann::json& json){
    text=json["text"].get<std::string>();
    explain=json["explain"].get<std::string>();
    auto& condition=json["condition"];
    if(condition["has_relic"].empty()==false){
        for(auto& need:condition["has_relic"]){
            NeedRelics.push_back(need.get<std::string>());
        }        
    }
    if(condition["not_has_relic"].empty()==false){
        for(auto& exclude:condition["not_has_relic"]){
            ExcludeRelics.push_back(exclude.get<std::string>());
        }        
    }
    if(json["outcome"].empty()==false){
        for(auto& effectID:json["outcome"]){
            effects.push_back(
                EffectManager::Get().getInstantEffect(effectID.get<std::string>())
            );        
        }
    }
}
const bool EventLevel::Option::isAvailable()const{
    for(const auto& need:NeedRelics){
        if(!StageController::Get().getPlayer().hasRelic(need)){
            return false; //如果需要的遗物不存在，则不可用
        }
    }
    for(const auto& exclude:ExcludeRelics){
        if(StageController::Get().getPlayer().hasRelic(exclude)){
            return false; //如果排除的遗物存在，则不可用
        }
    }
    return true;
}
void EventLevel::Option::apply(Player& player)const{
    for(const auto& effect:effects){
        if(effect){
            effect->onApply(player);
        }else{
            TraceLog(LOG_ERROR, "[DATA]: Invalid effect in event option: %s", text.c_str());
        }
    }
}
EventLevel::EventLevel(const nlohmann::json& events):Level(events){
    bool isJsonValid=Check::isJsonValid(events, {"id","background","text","options","title"});
    if(isJsonValid){
        for(const auto& option:events["options"]){
            if(!Check::isJsonValid(option, {"text","condition","outcome","explain"})){
                TraceLog(LOG_ERROR, "[DATA]: Invalid option in event level: %s", id.c_str());
                return;
            }else{
                if(!Check::isJsonValid(option["condition"], {"has_relic","not_has_relic"})){
                    TraceLog(LOG_ERROR, "[DATA]: Invalid condition in event option: %s", id.c_str());
                    return;
                }
            }
        }
    }else{
        TraceLog(LOG_ERROR, "[DATA]: Invalid event level data: %s", id.c_str());
        return;
    }
    id=events["id"].get<std::string>();
    title=events["title"].get<std::string>();
    background=ASSETS_IMAGE_PATH + events["background"].get<std::string>();
    text=events["text"].get<std::string>();
    for(const auto& optionJson:events["options"]){
        options.emplace_back(optionJson);
    }
    if(options.empty()){
        TraceLog(LOG_ERROR, "[DATA]: No valid options in event level: %s", id.c_str());
        return;
    }
}

void EventLevel::update(){
    EventUI& ui=EventUI::Get();
    ui.Draw();
    if(ui.isOptionChoosen()){
        int selectedOption=ui.selectedOption();
        if(selectedOption==-1){
            TraceLog(LOG_ERROR, "Option should be choosen but not");
        }
        options[selectedOption].apply(ctrl.getPlayer());
        TraceLog(LOG_INFO, "Choosed option %d",selectedOption);
        LevelManager::Get().toNextBattle();
    }
}
void EventLevel::onActivate(){
    Level::onActivate();
    setupUI();
}
void EventLevel::onDeactivate(){
    Level::onDeactivate();
    EventUI::Get().currentEvent=nullptr;
}
void EventLevel::setupUI()const{
    EventUI::Get().currentEvent=this;
    EventUI::Get().setup();
}