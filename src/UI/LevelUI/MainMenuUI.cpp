#include "UI/LevelUI/MainMenuUI.h"
#include "UI/UIUtility.h"
MainMenuUI::MainMenuUI():BaseUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	startButton={
		{
			screenWidth/2.f,
			screenHeight*0.5f,
			UI::ButtonCFG::BASIC_BUTTON_WIDTH,
			UI::ButtonCFG::BASIC_BUTTON_HEIGHT
		},
		L"开始游戏",
		ORANGE
	};
	exitButton={
		{
			screenWidth/2.f,
			screenHeight*0.7f,
			UI::ButtonCFG::BASIC_BUTTON_WIDTH,
			UI::ButtonCFG::BASIC_BUTTON_HEIGHT
		},
		L"离开",
		ORANGE
	};
	continueButton={
		{
			screenWidth/2.f,
			screenHeight*0.6f,
			UI::ButtonCFG::BASIC_BUTTON_WIDTH,
			UI::ButtonCFG::BASIC_BUTTON_HEIGHT
		},
		L"继续(不可用)",
		ORANGE
	};
	continueButton.setAvailibility(false);
}
void MainMenuUI::Draw()const{
	ClearBackground(WHITE);
	std::string title="Super Keyboard Legend Fans-made";
	UI::drawText(title,GetScreenWidth()/2.f,GetScreenHeight()*0.2f,2*UI::FontCFG::FONTSIZE,RED);
	startButton.Draw();
	exitButton.Draw();
	continueButton.Draw();
}
const bool MainMenuUI::isExit()const{
	return exitButton.isPressed();
}
const bool MainMenuUI::isStart()const{
	return startButton.isPressed();
}