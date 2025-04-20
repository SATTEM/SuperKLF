#ifndef GAMESTAGE_H
#define GAMESTAGE_H
#include "Entity.h"
#include <raylib.h>
#include <memory>


class StageController{
private:
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	StageController();
	~StageController()=default;
public:
	StageController(const StageController&)=delete;
	void operator=(const StageController&)=delete;
	static StageController& Get(){
		static StageController instance;
		return instance;
	}
	void resetGame();
	void setPlayer(std::unique_ptr<Player> p);
	void setEnemy(std::unique_ptr<Enemy> e);
	const std::vector<Bullet>& getPlayerBulletPattern() const{return player->getBulletPattern();}
	Player& getPlayer(){return *player;}
	Enemy& getEnemy(){return *enemy;}
};

#endif