#ifndef MAINMENULEVEL_H
#define MAINMENULEVEL_H
#include "Level.h"
class MainMenuLevel:public Level{
public:
    MainMenuLevel():Level(){id="MainMenu";}
    void update()override final;
};
#endif