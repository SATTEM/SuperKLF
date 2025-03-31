#include "DataManager.h"
#include "Entity.h"
#include "Event/EventFWD.h"
#include "Event/EventSystem.h"
#include "ResourceManager.h"
extern "C"{
	#include "raylib.h"
}
#include "GameStageFwd.h"
#include "GameStage.h"
#include "UI/UI.h"

void drawDefeatScreen(const DefeatUI& ui);
const GameStage checkBattleStage(const Player& player,const Enemy& enemy);

StageController::StageController(){
	currentStage=GameStage::MainMenu;
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
void StageController::beginBattle(){
	EventSystem::Get().broadcastEvent(Occasion::OnBattleStart,*player);
	EventSystem::Get().broadcastEvent(Occasion::OnBattleStart, *enemy);
	player->reset();
	enemy->reset();
}
void StageController::battleUpdate(){
	static bool continuedBattle=false;
	if(!continuedBattle){
		beginBattle();
		continuedBattle=true;
	}
	float deltaTime=GetFrameTime();
	player->Update(deltaTime);
	enemy->Update(deltaTime);
	BattleUI::Get().Draw();	
	currentStage=checkBattleStage(*player, *enemy);
	if(currentStage==GameStage::Defeat){
		TraceLog(LOG_INFO, "[GameSystem] Battle Defeated");
	}else if(currentStage==GameStage::Victory){
		TraceLog(LOG_INFO, "[GameSystem] Battle Victory");
	}
	if(currentStage!=GameStage::Battle&&currentStage!=GameStage::Pause){
		continuedBattle=false;
	}
}
const GameStage checkBattleStage(const Player& player,const Enemy& enemy){
	if(!player.isAlive()){return GameStage::Defeat;}
	if(!enemy.isAlive()){return GameStage::Victory;}
	return GameStage::Battle;
}
void StageController::defeatUpdate(){
	DefeatUI& ui=DefeatUI::Get();
	ui.Draw();
	if(IsKeyPressed(KEY_SPACE)){
		resetGame();
		transitionTo(GameStage::Battle);
		return;
	}
	if(ui.isRestart()){
		transitionTo(GameStage::MainMenu);
		return;
	}
	if(ui.isExit()){
		transitionTo(GameStage::Exit);
		return;
	}
}

void StageController::victoryUpdate(){
	VictoryUI& ui=VictoryUI::Get();
	if(DataManager::Get().getRefreshTimes()==0){
		ui.tryGenerateRewards(*player);
	}
	ui.Draw();
	if(ui.isRefreshButtonPressed()){
		ui.tryGenerateRewards(*player);
	}
	for(int i=0;i<3;i++){
		if(ui.isRewardButtonPressed(i)){
			ui.chooseReward(i,*player);
			currentStage=GameStage::NextLevel;
		}
	}
}
void StageController::nextLevel(){
	TraceLog(LOG_WARNING,"Next level has not been developed yet");
	DataManager::Get().levelAdvance();
	transitionTo(GameStage::Battle);
}
void StageController::pauseUpdate(){

}
void StageController::mainMenuUpdate(){
	MainMenuUI& ui=MainMenuUI::Get();
	ui.Draw();
	if(ui.isExit()){
		transitionTo(GameStage::Exit);
		return;
	}
	if(ui.isStart()){
		transitionTo(GameStage::Victory);
		return;
	}
}
void StageController::frameClean()const{
	ResourceManager::Get().cleanFont();
}