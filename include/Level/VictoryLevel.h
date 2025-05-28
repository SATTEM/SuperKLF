#ifndef VICTORYLEVLE_H
#define VICTORYLEVLE_H
#include "Level.h"

class VictoryLevel:public Level{
private:
    void tryGenerateRewards(Player& player);
    void chooseReward(const int i, Player& player);
public:
    VictoryLevel():Level(){id="Victory";}
    void update()override final;
};
#endif