#include "Level/VictoryLevel.h"
#include "UI/UI.h"
#include "DataManager.h"
#include "Level/LevelManager.h"
#include <random>
void VictoryLevel::update(){
    VictoryUI& ui=VictoryUI::Get();
    DataManager& data=DataManager::Get();
    if(data.getRefreshTimes()==0){
        tryGenerateRewards(ctrl.getPlayer());
    }
    ui.Draw();
    if(ui.isRefreshButtonPressed()){
        tryGenerateRewards(ctrl.getPlayer());
    }
    for(int i=0;i<3;i++){
        if(ui.isRewardButtonPressed(i)){
            chooseReward(i, ctrl.getPlayer());
            data.levelAdvance();
            LevelManager::Get().toNextBattle();
        }
    }
}
void VictoryLevel::tryGenerateRewards(Player& player){
    VictoryUI& ui=VictoryUI::Get();
	bool success=(player.money-DataManager::Get().getRefreshMoney())>=0;
	if(success){
        player.money-=DataManager::Get().getRefreshMoney();
		TraceLog(LOG_INFO,"Generating rewards");
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0,DataManager::Get().getRewardSize()-1);
		ui.currentRewards.clear();
		for(int i=0;i<3;i++){
			ui.currentRewards.push_back(DataManager::Get().getReward(dis(gen)));
			ui.rewardBtn[i].setText(ui.currentRewards[i].getName());
			ui.rewardBtn[i].setExplain(ui.currentRewards[i].getDescription());
		}
		DataManager::Get().refreshTimesAdvance();
		if(player.getMoney()<=DataManager::Get().getRefreshMoney()){
			ui.refreshBtn.setAvailibility(false);
		}else{
			ui.refreshBtn.setAvailibility(true);
		}
		ui.refreshBtn.setAddition(L"(-"+std::to_wstring(DataManager::Get().getRefreshMoney())+L")");
	}
}
void VictoryLevel::chooseReward(const int i, Player& player){
    VictoryUI& ui=VictoryUI::Get();
	TraceLog(LOG_INFO,"Choosing reward[%d]",i);
	DataManager::Get().resetRefreshTimes();
	Reward reward=ui.currentRewards[i];
	reward.apply(player);
}