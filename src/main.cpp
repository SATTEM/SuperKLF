#ifndef ASSETS_IMAGE_PATH
#define ASSETS_IMAGE_PATH
#include <exception>
#include <memory>
#endif

extern "C" {
	#include "raylib.h"
}
#include <fstream>
#include "Effect/EffectManager.h"
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

void PreInit();
void LateInit();
void clear();
const bool shouldEnd();

int main(void){
	PreInit();
	InitWindow(WindowWidth,WindowHeight,"SuperKLF");
	StageController& gameCTRL=StageController::Get();
	LateInit();
	SetTargetFPS(180);
	gameCTRL.transitionTo(GameStage::MainMenu);
	while(!shouldEnd()){
		BeginDrawing();
		ClearBackground(WHITE);
		DrawFPS(0, 0);
		gameCTRL.update();
		EndDrawing();
	}
	clear();
	CloseWindow();
}
const bool shouldEnd(){
	if(WindowShouldClose()){return true;}
	if(StageController::Get().getCurrentStage()==GameStage::Exit){
		return true;
	}
	return false;
}
//在raylib初始化前的初始化
void RegisterEffects();

void PreInit(){
	RegisterEffects();
}
//预初始化的实现
void RegisterEffects(){
	std::ifstream file(DATA::EFFECT_PATH);
	nlohmann::json effects=nlohmann::json::parse(file);
	EffectManager::Get().loadEffects(effects);
}
//在raylib初始化后的初始化
void entityInit();

void LateInit(){
	entityInit();
}
//后初始化的实现
void entityInit(){
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	player.reset(new Player( ASSETS_IMAGE_PATH"player.png",playerPos,10,0.1,100,10));
	enemy.reset(new Enemy { ASSETS_IMAGE_PATH"enemy.png",enemyPos});
	player->setOpponent(*enemy);
	player->addBullet(Bullet( ASSETS_IMAGE_PATH"pen.png",playerVel));
	player->setBlast(Blast( ASSETS_IMAGE_PATH"warning.png",playerVel));
	enemy->setOpponent(*player);
	enemy->addBullet({ ASSETS_IMAGE_PATH"pen.png"});
	StageController::Get().bindEntities(std::move(player),std::move(enemy));	
	TraceLog(LOG_INFO,"Entities initialized");
}
//清理函数
void clear(){
	ResourceManager::Get().cleanUp();
}