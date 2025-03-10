#ifndef GAMESTAGE_H
#define GAMESTAGE_H
#include "Entity.h"
#include "GameStageFwd.h"
#include <raylib.h>


class StageController{
private:
	GameStage currentStage;
	EventSignal signal;
	Player* player;
	Enemy* enemy;
	StageController();
	~StageController()=default;
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
	void bindEntities(Player& p,Enemy& e){player=&p;enemy=&e;}
	void update();
	const GameStage getCurrentStage() const {return currentStage;}
	void transitionTo(const GameStage stage){currentStage=stage;}
};

#endif