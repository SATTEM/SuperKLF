#include "Level/DefeatLevel.h"
#include "UI/LevelUI/DefeatUI.h"
#include "Level/LevelManager.h"
void DefeatLevel::update(){
    DefeatUI& ui=DefeatUI::Get();
    ui.Draw();
    if(ui.isRestart()){
        ctrl.resetGame();
        requestLevelChange("MainMenu");
    }else if(ui.isExit()){
        LevelManager::Get().setExitFlag();
    }
}