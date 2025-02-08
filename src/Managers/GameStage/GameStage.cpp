#include "Entity.h"
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
void handleDefeatInput();
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
	DefeatUI::Get().reset();
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
		case(GameStage::Pause):
		pauseUpdate();
		break;
		case(GameStage::EXIT):
		break;
	}	
}
void StageController::battleUpdate(){
	float deltaTime=GetFrameTime();
	player->Update(deltaTime);
	enemy->Update(deltaTime);
	player->Draw();
	enemy->Draw();
	currentStage=checkBattleStage(player, enemy);
}
const GameStage checkBattleStage(const Player* player,const Enemy* enemy){
	if(!player->isAlive()){return GameStage::Defeat;}
	if(!enemy->isAlive()){return GameStage::Victory;}
	return GameStage::Battle;
}
void StageController::defeatUpdate(){
	handleDefeatInput();
}
void handleDefeatInput(){
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
		break;
		case(EventSignal::EXIT):
		ctrl.transitionTo(GameStage::MainMenu);
		break;
		case(EventSignal::IDLE):
		break;
	}
}
void StageController::victoryUpdate(){
	DrawText("You Win!", GetScreenWidth()/2, GetScreenHeight()/2, 120, GOLD);
}
void StageController::pauseUpdate(){

}
void StageController::mainMenuUpdate(){
	transitionTo(GameStage::EXIT);
}