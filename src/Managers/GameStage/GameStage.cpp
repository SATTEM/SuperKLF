#include "DataManager.h"
#include "Entity.h"
#include <memory>
#include "GameStage.h"
#include "UI/UI.h"

void drawDefeatScreen(const DefeatUI& ui);

StageController::StageController(){
	player=nullptr;
	enemy=nullptr;
}
void StageController::setPlayer(std::unique_ptr<Player> p){
	player.reset(p.release());
}
void StageController::setEnemy(std::unique_ptr<Enemy> e){
	enemy.reset(e.release());
}
void StageController::resetGame(){
	if(player==nullptr||enemy==nullptr){return;}
	player->reset();
	enemy->reset();
	DataManager::Get().reset();
}