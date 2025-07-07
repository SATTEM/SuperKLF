#include "UI/LevelUI/EventUI.h"
#include "Level/EventLevel.h"
#include "UI/UIUtility.h"
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
	
	BaseUI::Draw();
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
