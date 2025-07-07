#ifndef EVENTUI_H
#define EVENTUI_H
#include "BaseUI.h"
#include "../UIwheel.h"
#include "../Button.h"
class EventLevel;
class EventUI:public BaseUI{
private:
	EventUI();
	~EventUI()=default;
	void setup();
	void Draw()const override;
	const bool isOptionChoosen()const;
	const int selectedOption()const;
	void drawEventDetail()const;	
public:
	static EventUI& Get(){
		static EventUI instance;
		return instance;
	}
private:
	//选项和按钮的映射
	std::map<int, int> btnToOption;
	Rectangle eventRect;
	Detail context;
	std::vector<ButtonWithExplain> optionButtons;
	const EventLevel* currentEvent=nullptr;	
	friend class EventLevel;
};

#endif