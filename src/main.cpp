#ifndef ASSETS_IMAGE_PATH
#define ASSETS_IMAGE_PATH
#include <exception>
#endif

extern "C" {
	#include "raylib.h"
}
#include <fstream>
#include "Effect/EffectManager.h"
#include "Event/EventSystem.h"
#include "DataManager.h"
#include "GameStageFwd.h"
#include "ResourceManager.h"
#include "GameStage.h"

const int WindowWidth=1280;
const int WindowHeight=720;

const Vector2 playerPos={640,500};
const Vector2 enemyPos={640,100};
const Vector2 playerVel={0,-1};
const Vector2 enemyVel={0,1};
using playerPtr=std::unique_ptr<Player>;
using enemyPtr=std::unique_ptr<Enemy>;

void RegisterEffects();
void entityInit(std::unique_ptr<Player>&,std::unique_ptr<Enemy>&);
bool shouldEnd();
int main(void){
	RegisterEffects();
	InitWindow(WindowWidth,WindowHeight,"SuperKLF");
	StageController& state=StageController::Get();
	ResourceManager& resMgr=ResourceManager::Get();
	EventSystem& eventSys=EventSystem::Get();
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	entityInit(player, enemy);
	SetTargetFPS(180);
	state.transitionTo(GameStage::Battle);
	while(!shouldEnd()){
		BeginDrawing();
		ClearBackground(WHITE);
		DrawFPS(0, 0);
		state.update();
		EndDrawing();
	}
	resMgr.cleanUp();
	CloseWindow();
}
void RegisterEffects(){
	std::ifstream file(DATA::EFFECT_PATH);
	nlohmann::json effects=nlohmann::json::parse(file);
	EffectManager::Get().loadEffects(effects);
}
bool shouldEnd(){
	if(WindowShouldClose()){return true;}
	if(StageController::Get().getCurrentStage()==GameStage::Exit){
		return true;
	}
	return false;
}

void entityInit(std::unique_ptr<Player>& player,std::unique_ptr<Enemy>& enemy){
	player.reset(new Player( ASSETS_IMAGE_PATH"player.png",playerPos,10,0.1,100,10));
	enemy.reset(new Enemy { ASSETS_IMAGE_PATH"enemy.png",enemyPos});
	player->setOpponent(*enemy);
	player->addBullet(Bullet( ASSETS_IMAGE_PATH"pen.png",playerVel));
	player->setBlast(Blast( ASSETS_IMAGE_PATH"warning.png",playerVel));
	enemy->setOpponent(*player);
	enemy->addBullet({ ASSETS_IMAGE_PATH"pen.png"});
	StageController::Get().bindEntities(*player, *enemy);	
	player->addRelic(EffectManager::Get().getRelicEffect("double_shoot_relic"));
	TraceLog(LOG_INFO,"Entities initialed");
}