#ifndef SHOPLEVEL_H
#define SHOPLEVEL_H
#include "Level.h"
//未完成
class ShopLevel:public Level{
public:
    ShopLevel(const nlohmann::json& json):Level(json){}
    ~ShopLevel()=default;
    void update()override final{}

};
#endif