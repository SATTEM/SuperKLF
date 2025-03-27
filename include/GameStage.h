#ifndef GAMESTAGE_H
#define GAMESTAGE_H
#include "Entity.h"
#include "GameStageFwd.h"
#include <raylib.h>
#include <memory>


class StageController{
private:
	GameStage currentStage;
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	StageController();
	~StageController()=default;
	void beginBattle();
	void battleUpdate();
	void mainMenuUpdate();
	void pauseUpdate();
	void victoryUpdate();
	void defeatUpdate();	
	void nextLevel();
public:
	StageController(const StageController&)=delete;
	void operator=(const StageController&)=delete;
	static StageController& Get(){
		static StageController instance;
		return instance;
	}
	void resetGame();
	void bindEntities(std::unique_ptr<Player>&& p,std::unique_ptr<Enemy>&& e){player.reset(p.release());enemy.reset(e.release());}
	const std::vector<Bullet>& getPlayerBulletPattern() const{return player->getBulletPattern();}
	Enemy& getEnemy(){return *enemy;}
	void update();
	const GameStage getCurrentStage() const {return currentStage;}
	void transitionTo(const GameStage stage){currentStage=stage;}
};

#endif