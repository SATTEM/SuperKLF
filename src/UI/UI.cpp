#include "UI/UI.h"
#include "Tools.h"
#include "Level/EventLevel.h"
#include "UI/UIUtility.h"
#include "UI/Information.h"
#include "DataManager.h"
#include <string>
extern "C"{
	#include "raylib.h"
}

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

VictoryUI::VictoryUI():BaseUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	refreshBtn={{screenWidth*0.8f,
		screenHeight*0.8f,
		UI::ButtonCFG::BASIC_BUTTON_WIDTH,
		UI::ButtonCFG::BASIC_BUTTON_HEIGHT},
		L"刷新",ORANGE};
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
void VictoryUI::Draw() const{
	ClearBackground(WHITE);
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	//胜利界面背景
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.5f));
	int passedLevel=DataManager::Get().getPassedLevel();
	if(passedLevel==0){
		std::string subTitle="选择你的初始奖励！";
		UI::drawText(subTitle,screenWidth/2.f,200,2*UI::FontCFG::FONTSIZE,RED);
	}else{
		std::string subTitle="你通过了"+std::to_string(passedLevel)+"关！";
		UI::drawText(subTitle,screenWidth/2.f,200,2*UI::FontCFG::FONTSIZE,RED);
	}
	
	for(int i=0;i<3;i++){
		rewardBtn[i].Draw();
	}
	refreshBtn.Draw();
}

BattleUI::BattleUI():BaseUI(),bulletPattern{Rectangle{0,GetScreenHeight()*0.34f,float(UI::BulletCFG::BULLET_DISPLAY_WIDTH),float(UI::BulletCFG::BULLET_DISPLAY_HEIGHT)}}
{}

void BattleUI::Draw() const{
	bulletPattern.Draw();
}

EventUI::EventUI():BaseUI(){
	eventRect={
		GetScreenWidth()*UI::EventCFG::EVENT_SCALE.x,
		GetScreenHeight()*UI::EventCFG::EVENT_SCALE.y,
		GetScreenWidth()*UI::EventCFG::EVENT_SCALE.width,
		GetScreenHeight()*UI::EventCFG::EVENT_SCALE.height
	};
	//说明文本的透明背景
	context.setBackgroundColor({0,0,0,0});
	context.setTextColor(BLACK);
	context.setFontSize(UI::EventCFG::EVENT_FONT_SIZE);
	context.setRectangle(eventRect);
}
void EventUI::setup(){
	//根据当前事件配置UI，仅添加有效的按钮
	btnToOption.clear();
	optionButtons.clear();
	if(currentEvent==nullptr){
		TraceLog(LOG_WARNING, "[LEVEL]: EventUI setup() called with null currentEvent");
		return;
	}
	int optCount=0;
	for(const auto& option: currentEvent->options){
		if(option.isAvailable()){
			// 创建按钮并添加到选项列表（逻辑不完善）
			ButtonWithExplain btn({
				GetScreenWidth()/2.f,
				GetScreenHeight()*0.5f + optionButtons.size() * (UI::ButtonCFG::BASIC_BUTTON_HEIGHT + 10),
				UI::ButtonCFG::BASIC_BUTTON_WIDTH,
				UI::ButtonCFG::BASIC_BUTTON_HEIGHT
			}, Trans::UTFTowstr(option.text), ORANGE, Trans::UTFTowstr(option.explain));
			btn.setAvailibility(option.isAvailable());
			optionButtons.push_back(btn);
			btnToOption[optionButtons.size()-1]=optCount;
		}
		optCount++;
	}
	context.setText(currentEvent->text);
}
void EventUI::Draw()const{
	ClearBackground(WHITE);
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	//事件大背景
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.5f));
	//绘制有效选项
	for(const auto& btn:optionButtons){
		btn.Draw();
	}
	//绘制事件文本背景（暂时不画）+文本
	drawEventDetail();
	//绘制事件标题
	UI::drawText(currentEvent->title, screenWidth/2.f, screenHeight*0.06f, 2*UI::FontCFG::FONTSIZE, RED);
	
}
void EventUI::drawEventDetail()const{
	//绘制事件描述背景(没做好)
	//绘制文本
	context.draw();
}
const bool EventUI::isOptionChoosen()const{
	for(const auto& btn:optionButtons){
		if(btn.isPressed()){return true;}
	}
	return false;
}
const int EventUI::selectedOption()const{
	for(size_t i=0;i<optionButtons.size();i++){
		if(optionButtons[i].isPressed()){
			return btnToOption.at(i);
		}
	}
	return -1;
}
