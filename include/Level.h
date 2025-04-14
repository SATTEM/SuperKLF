#ifndef LEVEL_H
#define LEVEL_H`
#include "nlohmann/json.hpp"
#include "Entity.h"
#include <string>
#include <fstream>

class Level{
private:
    int id=0;
public:
    Level(const nlohmann::json&);
    ~Level()=default;
};

class BattleLevel:public Level{
public:
    Enemy boss;
public:
    BattleLevel(const nlohmann::json& json);
};

class ShopLevel:public Level{};
class EventLevel:public Level{};

class LevelLoader{
private:
    LevelLoader()=default;
    ~LevelLoader()=default;
    //事件、敌人、商店的配置信息
    nlohmann::json events,battles,shop;
public:
    void operator=(const LevelLoader&)=delete;
    LevelLoader(const LevelLoader&)=delete;
    static LevelLoader& Get(){
        static LevelLoader instance;
        return instance;
    }
    void loadFromJson(const std::string& path);
    BattleLevel meetEnemy(const int id);
    ShopLevel meetShop();
    EventLevel meetEvent(const int id);
};


#endif