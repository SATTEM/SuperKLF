#include "GameStage.h"
int main(int argc,char* argv[]){
	StageController& gameCTRL=StageController::Get();
	gameCTRL.initGame(argc, argv);//初始化游戏
	gameCTRL.updateGame();//进入游戏循环
	gameCTRL.saveGame();
}