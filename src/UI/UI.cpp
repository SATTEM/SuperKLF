#include "UI/UI.h"
#include "UI/UIUtility.h"
#include "UI/Information.h"
#include "DataManager.h"
#include "Entity.h"
#include "RewardSystem.h"
#include <string>
#include <random>
#include <sys/select.h>
#include "UI/UIUtility.h"
extern "C"{
	#include "raylib.h"
}

MainMenuUI::MainMenuUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	startButton={
		{
			screenWidth/2.f,
			screenHeight*0.5f,
			UI::BASIC_BUTTON_WIDTH,
			UI::BASIC_BUTTON_HEIGHT
		},
		L"开始游戏",
		ORANGE
	};
	exitButton={
		{
			screenWidth/2.f,
			screenHeight*0.7f,
			UI::BASIC_BUTTON_WIDTH,
			UI::BASIC_BUTTON_HEIGHT
		},
		L"离开",
		ORANGE
	};
	continueButton={
		{
			screenWidth/2.f,
			screenHeight*0.6f,
			UI::BASIC_BUTTON_WIDTH,
			UI::BASIC_BUTTON_HEIGHT
		},
		L"继续(不可用)",
		ORANGE
	};
	continueButton.setAvailibility(false);
}
void MainMenuUI::Draw()const{
	ClearBackground(WHITE);
	std::string title="测试";
	UI::drawText(title,GetScreenWidth()/2.f,GetScreenHeight()*0.2f,2*UI::FONTSIZE,RED);
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

void DefeatUI::Draw() const{
	ClearBackground(WHITE);
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();	
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.5f));
	std::string title="Mamba Out!";
	std::string subTitle="你通过了"+std::to_string(DataManager::Get().getPassedLevel())+"关！";
	UI::drawText(title, screenWidth/2.f, screenHeight*0.2f, UI::FONTSIZE*2, RED);
	UI::drawText(subTitle, screenWidth/2.f, screenHeight*0.2f+150, UI::FONTSIZE, RED);
	restartButton.Draw();
	exitButton.Draw();
}

DefeatUI::DefeatUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	restartButton={
	{screenWidth/2.0f,
		screenHeight*0.6f,
		UI::BASIC_BUTTON_WIDTH,
		UI::BASIC_BUTTON_HEIGHT},
	L"[Space]Restart",ORANGE};
	exitButton={{
		screenWidth/2.0f,
		screenHeight*0.75f,
		UI::BASIC_BUTTON_WIDTH,
		UI::BASIC_BUTTON_HEIGHT},
		L"Quit",ORANGE};
}
const bool DefeatUI::isExit()const{
	return exitButton.isPressed();
}
const bool DefeatUI::isRestart()const{
	return restartButton.isPressed();
}

VictoryUI::VictoryUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	refreshBtn={{screenWidth*0.8f,
		screenHeight*0.8f,
		UI::BASIC_BUTTON_WIDTH,
		UI::BASIC_BUTTON_HEIGHT},
		L"刷新",ORANGE};
	float gap=(screenWidth-3*(UI::BASIC_BUTTON_WIDTH))/5.f;
	for(int i=0;i<3;i++){
		rewardBtn[i]={
			{gap*(i+2)+UI::BASIC_BUTTON_WIDTH*i,
			screenHeight*0.5f,
			UI::BASIC_BUTTON_WIDTH,
			UI::BASIC_BUTTON_HEIGHT},
			L"Reward",ORANGE,L"Explain"};
	}			
}
const bool VictoryUI::isRefreshButtonPressed()const{
	return refreshBtn.isPressed();
}
const bool VictoryUI::isRewardButtonPressed(int i)const{
	return rewardBtn[i].isPressed();
}
void VictoryUI::tryGenerateRewards(Player& player){
	bool success=player.deductMoney(DataManager::Get().getRefreshMoney());
	if(success){
		TraceLog(LOG_INFO,"Generating rewards");
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0,DataManager::Get().getRewardSize()-1);
		currentRewards.clear();
		for(int i=0;i<3;i++){
			currentRewards.push_back(DataManager::Get().getReward(dis(gen)));
			rewardBtn[i].setText(currentRewards[i].getName());
			rewardBtn[i].setExplain(currentRewards[i].getDescription());
		}
		DataManager::Get().refreshTimesAdvance();
		if(player.getMoney()<=DataManager::Get().getRefreshMoney()){
			refreshBtn.setAvailibility(false);
		}else{
			refreshBtn.setAvailibility(true);
		}
		refreshBtn.setAddition(L"(-"+std::to_wstring(DataManager::Get().getRefreshMoney())+L")");
	}
}
void VictoryUI::chooseReward(const int i,Player& player){
	TraceLog(LOG_INFO,"Choosing reward[%d]",i);
	DataManager::Get().resetRefreshTimes();
	Reward reward=currentRewards[i];
	reward.apply(player);
}

void VictoryUI::Draw() const{
	ClearBackground(WHITE);
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.5f));
	int passedLevel=DataManager::Get().getPassedLevel();
	if(passedLevel==0){
		std::string subTitle="选择你的初始奖励！";
		UI::drawText(subTitle,screenWidth/2.f,200,2*UI::FONTSIZE,RED);
	}else{
		std::string subTitle="你通过了"+std::to_string(passedLevel)+"关！";
		UI::drawText(subTitle,screenWidth/2.f,200,2*UI::FONTSIZE,RED);
	}
	
	for(int i=0;i<3;i++){
		rewardBtn[i].Draw();
	}
	refreshBtn.Draw();
}

BattleUI::BattleUI():bulletPattern{Rectangle{0,GetScreenHeight()*0.34f,float(UI::BULLET_DISPLAY_WIDTH),float(UI::BULLET_DISPLAY_HEIGHT)}}
{}

void BattleUI::Draw() const{
	bulletPattern.Draw();
}