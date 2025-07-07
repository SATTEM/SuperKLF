#ifndef VICTORYLEVLE_H
#define VICTORYLEVLE_H
#include "Level.h"
#include "RewardSystem.h"
#include <vector>

class VictoryLevel:public Level{
private:
    void tryGenerateRewards(Player& player);
    void chooseReward(const int i, Player& player);
public:
    VictoryLevel():Level(){id="Victory";}
    void update()override final;
private:
    std::vector<Reward> currentRewards;
};
#endif