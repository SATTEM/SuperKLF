#include "Level/MainMenuLevel.h"
#include "GameStage.h"
#include "UI/LevelUI/MainMenuUI.h"
#include "Level/LevelManager.h"
void MainMenuLevel::update(){
    MainMenuUI& ui=MainMenuUI::Get();
    ui.Draw();
    if(ui.isExit()){
        LevelManager::Get().setExitFlag();
    }else if(ui.isStart()){
        ctrl.resetGame();
        requestLevelChange("Victory");
    }else if(ui.isLoad()){
        ctrl.loadGame(DATA::FileCFG::savePath);
    }
}