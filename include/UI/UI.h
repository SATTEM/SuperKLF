#ifndef UI_H
#define UI_H
#include "Entity.h"
#include "RewardSystem.h"
#include <vector>
#include "Button.h"
#include "Information.h"
class BaseUI{
public:
	~BaseUI()=default;
	BaseUI()=default;
	BaseUI(const BaseUI&)=delete;
	const BaseUI& operator=(const BaseUI&)=delete;
	virtual void Draw()const =0;
};
class MainMenuUI:public BaseUI{
private:
	Button startButton,exitButton,continueButton;
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
};

class DefeatUI:public BaseUI{
private:
	Button restartButton,exitButton;
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
};
class VictoryUI:public BaseUI{
private:
	VictoryUI();
	~VictoryUI()=default;
	ButtonWithExplain rewardBtn[3];
	ButtonWithNumber refreshBtn;
	std::vector<Reward> currentRewards;
public:
	static VictoryUI& Get(){
		static VictoryUI instance;
		return instance;
	}
	void Draw() const override;
	void tryGenerateRewards(Player& player);
	void chooseReward(const int i,Player& player);
	const bool isRewardButtonPressed(int i) const;
	const bool isRefreshButtonPressed() const;
};

class BattleUI:public BaseUI{
private:
	BulletDisplay bulletPattern;
	BattleUI();
	~BattleUI()=default;
public:
	static BattleUI& Get(){
		static BattleUI instance;
		return instance;
	}
	void Draw() const override;
};

#endif