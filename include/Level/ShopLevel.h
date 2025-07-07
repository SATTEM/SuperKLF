#ifndef SHOPLEVEL_H
#define SHOPLEVEL_H
#include "Level.h"
#include "RewardSystem.h"
#include <vector>
//未完成
class ShopLevel:public Level{
private:
    void genGoods();
    void buyGoods(const int i);
public:
    ShopLevel():Level(){id="Shop";}
    ~ShopLevel()=default; 
    void update()override final;
    void onActivate()override final;
private:
    std::vector<Reward> currentRewards;
    std::vector<int> rewardsCost;
    friend class ShopUI;
};
#endif