#include "Level/Level.h"
#include "Level/LevelManager.h"
#include "GameStage.h"
#include "Tools.h"
#include "nlohmann/json.hpp"
#include <raylib.h>
#include <string>
Level::Level(const nlohmann::json& json):ctrl(StageController::Get()){
    if(Check::isJsonValid(json,{"id"})){
        id=json["id"].get<std::string>();
    }
}
void Level::requestLevelChange(const std::string& nextLevelID){
    LevelManager::Get().switchToLevel(nextLevelID);
}

