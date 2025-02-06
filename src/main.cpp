#ifndef ASSETS_PATH
#define ASSETS_PATH
#include <cstdint>
#include <memory>
#endif

extern "C" {
	#include "raylib.h"
}
#include "Bullet.h"
#include "Entity.h"
#include "ResourceManager.h"

const int WindowWidth=1280;
const int WindowHeight=720;

const Vector2 playerPos={640,500};
const Vector2 enemyPos={640,100};
const Vector2 playerVel={0,-1};
const Vector2 enemyVel={0,1};

void changeToBattle();
void entityInit(std::unique_ptr<Player>&,std::unique_ptr<Enemy>&);

int main(void){
	InitWindow(WindowWidth,WindowHeight,"SuperKLF");
	SetTargetFPS(180);
	ResourceManager& resMgr=ResourceManager::Get();
	changeToBattle();
	resMgr.cleanUp();
	CloseWindow();
}

void changeToBattle(){
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	entityInit(player, enemy);
	while(!WindowShouldClose()){
		float deltaTime =GetFrameTime();
		player->Update(deltaTime);
		enemy->Update(deltaTime);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawFPS(0, 0);
		player->Draw();
		enemy->Draw();
		EndDrawing();
	}
}
void entityInit(std::unique_ptr<Player>& player,std::unique_ptr<Enemy>& enemy){
	player.reset(new Player(ASSETS_PATH "player.png",playerPos,100,0.3,100,10));
	enemy.reset(new Enemy {ASSETS_PATH "enemy.png",enemyPos});
	player->setOpponent(*enemy);
	player->addBullet(Bullet(ASSETS_PATH "pen.png",playerVel));
	player->setBlast(Blast(ASSETS_PATH "warning.png",playerVel));
	enemy->setOpponent(*player);
	enemy->addBullet({ASSETS_PATH "pen.png"});	
}