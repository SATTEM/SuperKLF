#include "UI/LevelUI/BaseUI.h"
#include <raylib.h>
#include <string>
#include "UI/UIUtility.h"
#include "GameStage.h"
void BaseUI::Draw()const{
	//绘制金钱数
	std::string moneyText="金币："+std::to_string(StageController::Get().getPlayer().getMoney());
	UI::drawText(moneyText,GetScreenWidth()*0.1f,GetScreenHeight()*0.8f,UI::FontCFG::FONTSIZE,GOLD);
}