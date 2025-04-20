#ifndef LEVEL_H
#define LEVEL_H
#include "GameStage.h"
#include "nlohmann/json.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

class Level{
protected:
    std::string id;
    StageController& ctrl;
    bool active=false;
public:
    Level(const nlohmann::json&);
    Level():ctrl(StageController::Get()){}
    ~Level()=default;
    virtual void update()=0;
    virtual void onActivate(){active=true;}
    virtual void onDeactivate(){active=false;}
    void requestLevelChange(const std::string& nextLevelID);
    const std::string& getID()const{return id;}
};

class MainMenuLevel:public Level{
public:
    MainMenuLevel():Level(){id="MainMenu";}
    void update()override final;
};
class DefeatLevel:public Level{
public:
    DefeatLevel():Level(){id="Defeat";}
    void update()override final;
};
class VictoryLevel:public Level{
public:
    VictoryLevel():Level(){id="Victory";}
    void update()override final;
};
class ShopLevel:public Level{};
//需要动态加载的关卡
class EventLevel:public Level{
public:
    EventLevel(const nlohmann::json& json);
    void update()override final;
};
class BattleLevel:public Level{
private:
    std::string texPath,blast,name,blastImage;
    int max_energy,energy_rise,HP;
    float attack_interval=0;
    std::vector<std::string> bullets,relics;
public:
    BattleLevel(const nlohmann::json& json);
    void update()override final;
    void onActivate()override final;
};

class LevelManager{
private:
    LevelManager();
    ~LevelManager()=default;
    std::map<std::string, std::unique_ptr<Level>> levels;
    Level* currentLevel=nullptr;
    bool exitFlag=false;
    //事件、敌人、商店的配置信息
    nlohmann::json events,battles,shop;
public:
    void operator=(const LevelManager&)=delete;
    LevelManager(const LevelManager&)=delete;
    static LevelManager& Get(){
        static LevelManager instance;
        return instance;
    }
    void initialize();
    void switchToLevel(const std::string& id);
    void setExitFlag(){exitFlag=true;}
    void toNextBattle();
    Level* getCurrentLevel(){return currentLevel;}
    bool shouldExit()const{return exitFlag;}
private:
    void loadLevelFromJson();
};


#endif