#include "DataManager.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "UI/UIUtility.h"
#include "UI/UI.h"
#include "Effect/EffectManager.h"
#include <memory>
#include <fstream>
#include <ios>
#include <raylib.h>
#include "GameStage.h"
#include "Level/Level.h"
#include "Level/LevelManager.h"
const bool shouldEnd();
//初始化辅助函数
void RegisterEffects();//注册效果
void setTrace(const int,char*[]);//设置日志等级

StageController::StageController(){
	player=nullptr;
	enemy=nullptr;
}
void StageController::setEnemy(std::unique_ptr<Enemy> e){
	enemy.reset(e.release());
}
void StageController::initGame(int argc, char* argv[]){
	//初始化游戏
	setTrace(argc, argv);//设置日志等级
	RegisterEffects();//注册效果
	//初始化窗口
	InitWindow(UI::UICFG::WindowWidth, UI::UICFG::WindowHeight, "SuperKLF");
	//初始化玩家角色
	playerPos={
		GetScreenWidth()*UI::EntityCFG::DEFAULT_PLAYER_POSTION.x,
		GetScreenHeight()*UI::EntityCFG::DEFAULT_PLAYER_POSTION.y
	};
	const Vector2 playerVel={0,-1};
	const Vector2 enemyVel={0,1};
	player.reset(new Player( ASSETS_IMAGE_PATH"player.png",playerPos,DATA::PLAYER_HP,DATA::PLAYER_INTERVAL,DATA::PLAYER_ENERGY,DATA::PLAYER_RISE));
	player->addBullet(Bullet( ASSETS_IMAGE_PATH"pen.png",playerVel));
	player->setBlast(Blast( ASSETS_IMAGE_PATH"warning.png",playerVel));
	//初始化关卡
	LevelManager::Get().initialize();
	//设置FPS
	SetTargetFPS(240);
	TraceLog(LOG_INFO,"Game initialized");
}
void StageController::updateGame(){
	while(!shouldEnd()){
		BeginDrawing();
		ClearBackground(WHITE);
		//更新关卡指针
		Level* currentLevel=LevelManager::Get().getCurrentLevel();
		if(currentLevel){
			currentLevel->update();
			//绘制顶层元素（悬浮元素）
			UI::drawTop();
		}
		DrawFPS(0, 0);
		EndDrawing();
		//每帧结束的清理函数
		ResourceManager::Get().frameClean();
	}
	//游戏结束，清理资源
	ResourceManager::Get().cleanUp();
}
void StageController::resetGame(){
//重置游戏
	if(player==nullptr||enemy==nullptr){return;}
	player->reset();
	enemy->reset();
	DataManager::Get().reset();
}
void StageController::captureGame(){
	save["data"]=DataManager::Get().dump();
	save["player"]=player->dump();
}
void StageController::saveGame(const std::string& path)const{
	if(save.empty()){return;}
	if(save.contains("currentLevel")==false){return;}
	if(save.contains("player")==false){return;}
	std::ofstream os(path,std::ios_base::out);
	os<<save;
	os.close();
}
void StageController::loadGame(const std::string& savePath){
	using json=nlohmann::json;
	std::ifstream is(savePath);
	json save;
	is>>save;
	is.close();
	DataManager::Get().load(save["data"]);
	player.reset(new Player(save["player"],playerPos));
	LevelManager::Get().switchToLevel(save["currentLevel"].get<std::string>());
	resetGame();
}
const bool shouldEnd(){
	return WindowShouldClose()||LevelManager::Get().shouldExit();
}
void setTrace(const int argc,char* cmd[]){
	if(argc==1){
		SetTraceLogLevel(LOG_ERROR);
		return;
	}
	std::string level=cmd[1];
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