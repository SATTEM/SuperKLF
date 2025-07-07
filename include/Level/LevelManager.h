#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H
#include "Level.h"

class LevelManager{
private:
    LevelManager();
    ~LevelManager()=default;
    void loadLevelFromJson();
    const bool shouldEnterEvent()const;
    const bool shouldEnterShop()const;
    const std::string generateEventID()const;    
    const std::string generateBattleID()const;
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
    std::map<std::string, std::unique_ptr<Level>> levels;
    Level* currentLevel=nullptr;
    bool exitFlag=false;
    int eventCount=0,battleCount=0;
};


#endif