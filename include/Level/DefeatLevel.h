#ifndef DEFEATLEVEL_H
#define DEFEATLEVEL_H
#include "Level.h"
class DefeatLevel:public Level{
public:
    DefeatLevel():Level(){id="Defeat";}
    void update()override final;
};

#endif