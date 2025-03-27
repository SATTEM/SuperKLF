#ifndef UI_H
#define UI_H
#include "Entity.h"
#include "RewardSystem.h"
#include <vector>
#include "UIComponent.h"
namespace UI {
	const int MAX_ENGLISH_EXPLAIN_LINE=24;
	const int MIN_FONT_SIZE=1;	
	const int FONTSIZE=40;
	const int EXPLAIN_FONTSIZE=15;
	const int BULLET_DISPLAY_OFFSET=2;	
	const int BULLET_DISPLAY_HEIGHT=240;
	const int BULLET_DISPLAY_WIDTH=120;
	const float BULLET_DISPLAY_SCALE=0.15;
}
class DefeatUI{
private:
	Button restartButton,exitButton;
	DefeatUI();
	~DefeatUI()=default;
public:
	DefeatUI(const DefeatUI&)=delete;
	void operator=(const DefeatUI&)=delete;
	static DefeatUI& Get(){
		static DefeatUI instance;
		return instance;
	}
	const Button& getRestartBtn() const{return restartButton;}
	const Button& getExitBtn() const{return exitButton;}
	void Draw() const;
};
class VictoryUI{
private:
	VictoryUI();
	~VictoryUI()=default;
	ButtonWithExplain rewardBtn[3];
	ButtonWithNumber refreshBtn;
	std::vector<Reward> currentRewards;
public:
	VictoryUI(const VictoryUI&)=delete;
	void operator=(const VictoryUI&)=delete;
	static VictoryUI& Get(){
		static VictoryUI instance;
		return instance;
	}
	void Draw() const;
	void tryGenerateRewards(Player& player);
	void chooseReward(const int i,Player& player);
	const Button& getRewardBtn(int i) const{return rewardBtn[i];}
	const ButtonWithNumber& getRefreshBtn() const{return refreshBtn;}
};

class BattleUI{
private:
	BulletDisplay bulletPattern;
	BattleUI();
	~BattleUI()=default;
public:
	BattleUI(const BattleUI&)=delete;
	void operator=(const BattleUI&)=delete;
	static BattleUI& Get(){
		static BattleUI instance;
		return instance;
	}
	void Draw() const;
};

#endif