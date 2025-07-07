#include "UI/LevelUI/VictoryUI.h"
#include "DataManager.h"
#include "Tools.h"
#include "UI/UIUtility.h"
#include <string>
VictoryUI::VictoryUI():BaseUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	refreshBtn={
		{screenWidth*0.8f,
		screenHeight*0.8f,
		UI::ButtonCFG::BASIC_BUTTON_WIDTH,
		UI::ButtonCFG::BASIC_BUTTON_HEIGHT},
		L"刷新",ORANGE};
	std::wstring skipText=L"跳过(+"+Trans::UTFTowstr(std::to_string(DATA::SKIP_MONEY))+L"金币)";
	skipBtn={
		{screenWidth*0.5f,
			screenHeight*0.8f,
			UI::ButtonCFG::BASIC_BUTTON_WIDTH,
			UI::ButtonCFG::BASIC_BUTTON_HEIGHT},
		skipText,ORANGE
	};
	float gap=(screenWidth-3*(UI::ButtonCFG::BASIC_BUTTON_WIDTH))/5.f;
	for(int i=0;i<3;i++){
		rewardBtn[i]={
			{gap*(i+2)+UI::ButtonCFG::BASIC_BUTTON_WIDTH*i,
			screenHeight*0.5f,
			UI::ButtonCFG::BASIC_BUTTON_WIDTH,
			UI::ButtonCFG::BASIC_BUTTON_HEIGHT},
			L"Reward",ORANGE,L"Explain"};
	}			
}
const bool VictoryUI::isRefreshButtonPressed()const{
	return refreshBtn.isPressed();
}
const bool VictoryUI::isRewardButtonPressed(int i)const{
	return rewardBtn[i].isPressed();
}
const bool VictoryUI::isSkipButtonPressed()const{
	return skipBtn.isPressed();
}
void VictoryUI::Draw() const{
	ClearBackground(WHITE);
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	//胜利界面背景
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.5f));
	int passedLevel=DataManager::Get().getPassedLevel();
	if(passedLevel==0){
		std::string subTitle="选择你的初始奖励！";
		UI::drawText(subTitle,screenWidth/2.f,screenHeight*0.3f,2*UI::FontCFG::FONTSIZE,RED);
	}else{
		std::string subTitle="你通过了"+std::to_string(passedLevel)+"关！";
		UI::drawText(subTitle,screenWidth/2.f,screenHeight*0.3f,2*UI::FontCFG::FONTSIZE,RED);
	}
	
	for(int i=0;i<3;i++){
		rewardBtn[i].Draw();
	}
	refreshBtn.Draw();
	skipBtn.Draw();
	BaseUI::Draw();
}
