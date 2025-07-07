#include "UI/LevelUI/BattleUI.h"
#include "UI/LevelUI/BaseUI.h"
BattleUI::BattleUI():BaseUI(),bulletPattern{Rectangle{0,GetScreenHeight()*0.34f,float(UI::BulletCFG::BULLET_DISPLAY_WIDTH),float(UI::BulletCFG::BULLET_DISPLAY_HEIGHT)}}
{}

void BattleUI::Draw() const{
	bulletPattern.Draw();
	BaseUI::Draw();
}