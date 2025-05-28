#include "Level/MainMenuLevel.h"
#include "UI/UI.h"
#include "Level/LevelManager.h"
void MainMenuLevel::update(){
    MainMenuUI& ui=MainMenuUI::Get();
    ui.Draw();
    if(ui.isExit()){
        LevelManager::Get().setExitFlag();
    }else if(ui.isStart()){
        ctrl.resetGame();
        requestLevelChange("Victory");
    }
}