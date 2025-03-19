#include "DataManager.h"
#include "Entity.h"
#include "Event/EventFWD.h"
#include "Event/EventSystem.h"
extern "C"{
	#include "raylib.h"
}
#include "GameStageFwd.h"
#include "GameStage.h"
#include "UI.h"
#include "Collision.h"

using Collision::checkIsTouchButton;


void responseToSignalFromDefeat(const EventSignal signal);
void drawDefeatScreen(const DefeatUI& ui);
const GameStage checkBattleStage(const Player* player,const Enemy* enemy);

StageController::StageController(){
	currentStage=GameStage::MainMenu;
	signal=EventSignal::IDLE;
	player=nullptr;
	enemy=nullptr;
}
void StageController::resetGame(){
	if(player==nullptr||enemy==nullptr){return;}
	player->reset();
	enemy->reset();
	DataManager::Get().reset();
}
void StageController::update(){
	switch(currentStage){
		case(GameStage::MainMenu):
		mainMenuUpdate();
		break;
		case(GameStage::Battle):
		battleUpdate();
		break;
		case(GameStage::Victory):
		victoryUpdate();
		break;
		case(GameStage::Defeat):
		defeatUpdate();
		break;
		case(GameStage::NextLevel):
		nextLevel();
		break;
		case(GameStage::Pause):
		pauseUpdate();
		break;
		case(GameStage::Exit):
		break;
	}	
}
void StageController::battleUpdate(){
	static bool newBattle=false;
	if(newBattle){
		EventSystem::Get().broadcastEvent(Occasion::OnBattleStart,*player);
		EventSystem::Get().broadcastEvent(Occasion::OnBattleStart, *enemy);
		newBattle=false;
	}
	float deltaTime=GetFrameTime();
	player->Update(deltaTime);
	enemy->Update(deltaTime);
	player->Draw();
	enemy->Draw();
	currentStage=checkBattleStage(player, enemy);
	if(currentStage==GameStage::Defeat){
		TraceLog(LOG_INFO, "[GameSystem] Battle Defeated");
	}else if(currentStage==GameStage::Victory){
		TraceLog(LOG_INFO, "[GameSystem] Battle Victory");
	}
	if(currentStage!=GameStage::Battle&&currentStage!=GameStage::Pause){
		newBattle=true;
	}
}
const GameStage checkBattleStage(const Player* player,const Enemy* enemy){
	if(!player->isAlive()){return GameStage::Defeat;}
	if(!enemy->isAlive()){return GameStage::Victory;}
	return GameStage::Battle;
}
void StageController::defeatUpdate(){
	DefeatUI& ui=DefeatUI::Get();
	ui.Draw();
	if(IsKeyPressed(KEY_SPACE)){
		responseToSignalFromDefeat(EventSignal::RESTART);
		return;
	}
	if(checkIsTouchButton(ui.getRestartBtn())){
		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			responseToSignalFromDefeat(EventSignal::RESTART);
		}
	}
	if(checkIsTouchButton(ui.getExitBtn())){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			responseToSignalFromDefeat(EventSignal::EXIT);
		}
	}
}
void responseToSignalFromDefeat(const EventSignal signal){
	StageController& ctrl=StageController::Get();
	switch(signal){
		case(EventSignal::RESTART):
		ctrl.resetGame();
		ctrl.transitionTo(GameStage::Battle);
		TraceLog(LOG_INFO, "[GameSystem] Restart game");
		break;
		case(EventSignal::EXIT):
		ctrl.transitionTo(GameStage::MainMenu);
		TraceLog(LOG_INFO, "[GameSystem] Return to main menu");
		break;
		case(EventSignal::IDLE):
		break;
	}
}
void StageController::victoryUpdate(){
	VictoryUI& ui=VictoryUI::Get();
	if(DataManager::Get().getRefreshTimes()==0){
		ui.tryGenerateRewards(*player);
	}
	ui.Draw();
	if(checkIsTouchButton(ui.getRefreshBtn())){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			ui.tryGenerateRewards(*player);
		}
	}
	for(int i=0;i<3;i++){
		if(checkIsTouchButton(ui.getRewardBtn(i))){
			if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
				ui.chooseReward(i,*player);
				currentStage=GameStage::NextLevel;
			}
		}
	}
}
void StageController::nextLevel(){
	TraceLog(LOG_INFO,"[GameSystem] Advancing to next Level");
	TraceLog(LOG_WARNING,"Next level has not been developed yet");
	transitionTo(GameStage::Battle);
}
void StageController::pauseUpdate(){

}
void StageController::mainMenuUpdate(){
	transitionTo(GameStage::Exit);
}