#ifndef BATTLELEVEL_H
#define BATTLELEVEL_H
#include "Level.h"

class BattleLevel:public Level{
private:
    void getMoneyReward()const;
public:
    BattleLevel(const nlohmann::json& json);
    void update()override final;
    void onActivate()override final;
private:
    std::string texPath,blast,name,blastImage;
    int max_energy,energy_rise,HP;
    float attack_interval=0;
    std::vector<std::string> bullets,relics;    
};

#endif