#include "UI/LevelUI/DefeatUI.h"
#include "DataManager.h"
#include "UI/LevelUI/BaseUI.h"
#include "UI/UIUtility.h"
void DefeatUI::Draw() const{
	ClearBackground(WHITE);
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();	
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.5f));
	std::string title="Mamba Out!";
	std::string subTitle="你通过了"+std::to_string(DataManager::Get().getPassedLevel())+"关！";
	UI::drawText(title, screenWidth/2.f, screenHeight*0.2f, UI::FontCFG::FONTSIZE*2, RED);
	UI::drawText(subTitle, screenWidth/2.f, screenHeight*0.2f+150, UI::FontCFG::FONTSIZE, RED);
	restartButton.Draw();
	exitButton.Draw();
	BaseUI::Draw();
}

DefeatUI::DefeatUI():BaseUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	restartButton={
	{screenWidth/2.0f,
		screenHeight*0.6f,
		UI::ButtonCFG::BASIC_BUTTON_WIDTH,
		UI::ButtonCFG::BASIC_BUTTON_HEIGHT},
	L"[空格]主菜单",ORANGE};
	exitButton={{
		screenWidth/2.0f,
		screenHeight*0.75f,
		UI::ButtonCFG::BASIC_BUTTON_WIDTH,
		UI::ButtonCFG::BASIC_BUTTON_HEIGHT},
		L"离开游戏",ORANGE};
}
const bool DefeatUI::isExit()const{
	return exitButton.isPressed();
}
const bool DefeatUI::isRestart()const{
	return restartButton.isPressed()||IsKeyPressed(KEY_SPACE);
}