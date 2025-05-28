#include "UI/UIUtility.h"
extern "C" {
	#include "raylib.h"
}
#include "Level/Level.h"
#include "Level/LevelManager.h"
#include <fstream>
#include <memory>
#include <string>
#include "Effect/EffectManager.h"
#include "DataManager.h"
#include "ResourceManager.h"
#include "GameStage.h"

const int WindowWidth=1280;
const int WindowHeight=720;

const Vector2 playerVel={0,-1};
const Vector2 enemyVel={0,1};

void PreInit(int argc,char* argv[]);
void LateInit();
void clear();
const bool shouldEnd();

int main(int argc,char* argv[]){
	PreInit(argc,argv);
	InitWindow(WindowWidth,WindowHeight,"SuperKLF");
	StageController& gameCTRL=StageController::Get();
	LevelManager& levelManager=LevelManager::Get();
	//后初始化，其中设置了player的数值
	LateInit();
	while(!shouldEnd()){
		BeginDrawing();
		ClearBackground(WHITE);
		Level* currentLevel=LevelManager::Get().getCurrentLevel();
		if(currentLevel){
			currentLevel->update();
			UI::drawTop();
		}
		DrawFPS(0, 0);
		EndDrawing();
		ResourceManager::Get().frameClean();
	}
	clear();
	CloseWindow();
}
const bool shouldEnd(){
	return WindowShouldClose()||LevelManager::Get().shouldExit();
}
//在raylib初始化前的初始化
void RegisterEffects();
void setTrace(const int,char[]);
void PreInit(int argc,char* argv[]){
	setTrace(argc,argv[1]);
	RegisterEffects();
}
//预初始化的实现
void setTrace(const int argc,char cmd[]){
	if(argc==1){
		SetTraceLogLevel(LOG_ERROR);
		return;
	}
	std::string level=cmd;
	if(level=="--info"){
		SetTraceLogLevel(LOG_INFO);
	}else if(level=="--warning"){
		SetTraceLogLevel(LOG_WARNING);
	}else{
		SetTraceLogLevel(LOG_ERROR);
	}
}
void RegisterEffects(){
	std::ifstream file(DATA::EFFECT_PATH);
	nlohmann::json effects=nlohmann::json::parse(file);
	EffectManager::Get().loadEffects(effects);
}
//在raylib初始化后的初始化
void entityInit();

void LateInit(){
	UI::EntityCFG::DEFAULT_ENEMY_POSITION={GetScreenWidth()/2.f,GetScreenHeight()*0.2f};
	UI::EntityCFG::DEFAULT_PLAYER_POSTION={GetScreenWidth()/2.f,GetScreenHeight()*0.7f};
	entityInit();
	LevelManager::Get().initialize();
	SetTargetFPS(240);
}
//后初始化的实现
void entityInit(){
	std::unique_ptr<Player> player;
	player.reset(new Player( ASSETS_IMAGE_PATH"player.png",UI::EntityCFG::DEFAULT_PLAYER_POSTION,DATA::PLAYER_HP,DATA::PLAYER_INTERVAL,DATA::PLAYER_ENERGY,DATA::PLAYER_RISE));
	player->addBullet(Bullet( ASSETS_IMAGE_PATH"pen.png",playerVel));
	player->setBlast(Blast( ASSETS_IMAGE_PATH"warning.png",playerVel));
	StageController::Get().setPlayer(std::move(player));
	TraceLog(LOG_INFO,"Entities initialized");
}
//清理函数
void clear(){
	ResourceManager::Get().cleanUp();
}