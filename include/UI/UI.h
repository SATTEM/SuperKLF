#ifndef UI_H
#define UI_H
#include "RewardSystem.h"
#include <map>
#include <raylib.h>
#include <vector>
#include "Button.h"
#include "Information.h"
class EventLevel;
//UI功能:绘制、检测并返回状态
class BaseUI{
public:
	virtual ~BaseUI()=default;
	BaseUI()=default;
	BaseUI(const BaseUI&)=delete;
	const BaseUI& operator=(const BaseUI&)=delete;
	virtual void Draw()const =0;
};
class MainMenuUI:public BaseUI{
private:
	MainMenuUI();
	~MainMenuUI()=default;
public:
	static MainMenuUI& Get(){
		static MainMenuUI instance;
		return instance;
	}
	void Draw()const override final;
	const bool isExit()const;
	const bool isStart()const;
private:
	Button startButton,exitButton,continueButton;
};

class DefeatUI:public BaseUI{
private:
	DefeatUI();
	~DefeatUI()=default;
public:
	static DefeatUI& Get(){
		static DefeatUI instance;
		return instance;
	}
	const bool isRestart() const;
	const bool isExit() const;
	void Draw() const override;
private:	
	Button restartButton,exitButton;
};
class VictoryUI:public BaseUI{
private:
	VictoryUI();
	~VictoryUI()=default;
	const bool isRewardButtonPressed(int i) const;
	const bool isRefreshButtonPressed() const;
public:
	static VictoryUI& Get(){
		static VictoryUI instance;
		return instance;
	}
	void Draw() const override;
private:
	ButtonWithExplain rewardBtn[3];
	ButtonWithNumber refreshBtn;
	std::vector<Reward> currentRewards;	
	friend class VictoryLevel;
};

class BattleUI:public BaseUI{
private:
	BattleUI();
	~BattleUI()=default;
public:
	static BattleUI& Get(){
		static BattleUI instance;
		return instance;
	}
	void Draw() const override;
private:
	BulletDisplay bulletPattern;	
};
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