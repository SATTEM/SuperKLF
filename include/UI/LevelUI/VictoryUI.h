#ifndef VICTORYUI_H
#define VICTORYUI_H
#include "BaseUI.h"
#include "../Button.h"
class VictoryUI:public BaseUI{
private:
	VictoryUI();
	~VictoryUI()=default;
	const bool isRewardButtonPressed(int i) const;
	const bool isRefreshButtonPressed() const;
	const bool isSkipButtonPressed()const;
public:
	static VictoryUI& Get(){
		static VictoryUI instance;
		return instance;
	}
	void Draw() const override;
private:
	ButtonWithExplain rewardBtn[3];
	ButtonWithNumber refreshBtn;
	Button skipBtn;
	friend class VictoryLevel;
};
#endif