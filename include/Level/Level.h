#ifndef LEVEL_H
#define LEVEL_H
#include "GameStage.h"
#include "nlohmann/json.hpp"
#include <string>
class Level{
protected:
    std::string id;
    StageController& ctrl;
    bool active=false;
public:
    Level(const nlohmann::json&);
    Level():ctrl(StageController::Get()){}
    virtual ~Level(){active=false;};
    virtual void update()=0;
    virtual void onActivate(){active=true;}
    virtual void onDeactivate(){active=false;}
    void requestLevelChange(const std::string& nextLevelID);
    const std::string& getID()const{return id;}
};
#endif