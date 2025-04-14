#include "Level.h"
#include "Tools.h"
Level::Level(const nlohmann::json& json){
    if(Check::isJsonValid(json,{"id"})){
        id=json.get<int>();
    }
}
BattleLevel::BattleLevel(const nlohmann::json& json):Level(json){
    bool notValid=!Check::isJsonValid(json,
    {"id","image","HP","bullets","relic_id",
    "max_energy","energy_rise","attack_interval",
    "blast"});
    if(notValid){return;}
    
}