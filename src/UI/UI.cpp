#include "UI.h"
#include "DataManager.h"
#include "Entity.h"
#include "RewardSystem.h"
#include "UIComponent.h"
#include <string>
#include <random>
extern "C"{
	#include "raylib.h"
}

void DefeatUI::Draw() const{
	ClearBackground(WHITE);
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();	
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.5f));
	std::string title="Mamba Out!";
	std::string subTitle=R"(You have passed %d levels!)";
	DrawText(title.c_str(),UI::countTextPosX(title,screenWidth/2,2*UI::FONTSIZE),200,2*UI::FONTSIZE,RED);
	DrawText(TextFormat(subTitle.c_str(),DataManager::Get().getPassedLevel()), UI::countTextPosX(subTitle, screenWidth/2, UI::FONTSIZE), 350, UI::FONTSIZE, RED);
	restartButton.Draw();
	exitButton.Draw();
}

DefeatUI::DefeatUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	restartButton={
	{screenWidth/2.0f-UI::BASIC_BUTTON_WIDTH/2.0f,
		screenHeight*0.6f,
		UI::BASIC_BUTTON_WIDTH,
		UI::BASIC_BUTTON_HEIGHT},
	"[Space]Restart",ORANGE};
	exitButton={{
		screenWidth/2.0f-UI::BASIC_BUTTON_WIDTH/2.0f,
		screenHeight*0.75f,
		UI::BASIC_BUTTON_WIDTH,
		UI::BASIC_BUTTON_HEIGHT},
		"Quit",ORANGE};
}
VictoryUI::VictoryUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	refreshBtn={{screenWidth/2.0f-UI::BASIC_BUTTON_WIDTH/2.0f,
		screenHeight*0.8f,
		UI::BASIC_BUTTON_WIDTH,
		UI::BASIC_BUTTON_HEIGHT},
		"Refresh",ORANGE};
	for(int i=0;i<3;i++){
		rewardBtn[i]={
			{GetScreenWidth()/5.0f*(i+2)-UI::BASIC_BUTTON_WIDTH,
			GetScreenHeight()*0.5f,
			UI::BASIC_BUTTON_WIDTH,
			UI::BASIC_BUTTON_HEIGHT},
			"Reward",ORANGE,"Explain"};
	}			
}
void VictoryUI::tryGenerateRewards(Player& player){
	TraceLog(LOG_INFO,"Generating rewards");
	player.deductMoney(DataManager::Get().getRefreshMoney());
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0,DataManager::Get().getRewardSize()-1);
	currentRewards.clear();
	for(int i=0;i<3;i++){
		currentRewards.push_back(DataManager::Get().getReward(dis(gen)));
		rewardBtn[i].setText(currentRewards[i].name);
		rewardBtn[i].setExplain(currentRewards[i].description);
	}
	DataManager::Get().refreshTimesAdvance();
	if(player.getMoney()<=DataManager::Get().getRefreshMoney()){
		refreshBtn.setAvailibility(false);
	}else{
		refreshBtn.setAvailibility(true);
	}
	refreshBtn.setAddition("(-"+std::to_string(DataManager::Get().getRefreshMoney())+")");
}
void VictoryUI::chooseReward(const int i,Player& player){
	TraceLog(LOG_INFO,"Choosing reward[%d]",i);
	DataManager::Get().resetRefreshTimes();
	Reward reward=currentRewards[i];
}

void VictoryUI::Draw() const{
	ClearBackground(WHITE);
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.5f));
	std::string subTitle=R"(You passed %d levels!)";
	DrawText(TextFormat(subTitle.c_str(),DataManager::Get().getPassedLevel()), UI::countTextPosX(subTitle, screenWidth/2, 2*UI::FONTSIZE), 200, 2*UI::FONTSIZE, RED);
	for(int i=0;i<3;i++){
		rewardBtn[i].Draw();
	}
	refreshBtn.Draw();
}