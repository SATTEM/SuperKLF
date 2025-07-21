#ifndef GAMESTAGE_H
#define GAMESTAGE_H
#include "Entity.h"
#include <raylib.h>
#include <memory>
namespace DATA{
	namespace FileCFG {
		const std::string savePath="./save";
	}
}
class Level;

class StageController{
private:
	StageController();
	~StageController()=default;
public:
	StageController(const StageController&)=delete;
	void operator=(const StageController&)=delete;
	static StageController& Get(){
		static StageController instance;
		return instance;
	}
	void initGame(int argc, char* argv[]);
	void captureGame();
	void captureBattle(const std::string& battleID){save["currentLevel"]=battleID;}
	void updateGame();
	void resetGame();
	void saveGame(const std::string& path=DATA::FileCFG::savePath)const;
	void loadGame(const std::string& savePath);
	void setEnemy(std::unique_ptr<Enemy> e);
	const std::vector<Bullet>& getPlayerBulletPattern() const{return player->getBulletPattern();}
	Player& getPlayer(){return *player;}
	Enemy& getEnemy(){return *enemy;}
private:
	Vector2 playerPos;
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;	
	nlohmann::json save;
};

#endif