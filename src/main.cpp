#ifndef ASSETS_PATH
#define ASSETS_PATH
#endif

extern "C" {
	#include "raylib.h"
}
#include "Bullet.h"
#include "Entity.h"
#include "ResourceManager.h"


int main(void){
	const int width=1280;
	const int height=720;
	ResourceManager& resMgr=ResourceManager::Get();
	InitWindow(width,height,"SuperKLF");
	Vector2 playerPos={640,500};
	Vector2 enemyPos={640,100};
	Player* player=new Player{ASSETS_PATH "player.png",playerPos,100,0.5f};
	player->addBullet({ASSETS_PATH "pen.png",{0,-1}});
	Enemy* enemy=new Enemy {ASSETS_PATH "enemy.png",enemyPos};
	enemy->addBullet({ASSETS_PATH "pen.png"});
	SetTargetFPS(180);
	while(!WindowShouldClose()){
		
		float deltaTime =GetFrameTime();
		player->Update(*enemy,deltaTime);
		enemy->Update(*player,deltaTime);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawFPS(0, 0);
		player->Draw();
		enemy->Draw();
		EndDrawing();
	}
	//在CloseWindow()清除所有raylib资源，使相关函数不可用之前，
	//把player和enemy占用的资源卸载
	delete player;
	delete enemy;
	resMgr.cleanUp();
	CloseWindow();
}
