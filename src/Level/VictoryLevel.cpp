#include "Level/VictoryLevel.h"
#include "EntityModifier.h"
#include "UI/LevelUI/VictoryUI.h"
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
	if(ui.isSkipButtonPressed()){
		EntityModifier::addMoney(ctrl.getPlayer(), DATA::SKIP_MONEY);
		data.levelAdvance();
		LevelManager::Get().toNextBattle();
	}
}
void VictoryLevel::tryGenerateRewards(Player& player){
    VictoryUI& ui=VictoryUI::Get();
	bool success=(player.getMoney()-DataManager::Get().getRefreshMoney())>=0;
	if(success){
		EntityModifier::addMoney(player, -DataManager::Get().getRefreshMoney());
		TraceLog(LOG_INFO,"Generating rewards");
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0,DataManager::Get().getRewardSize()-1);
		currentRewards.clear();
		for(int i=0;i<3;i++){
			currentRewards.push_back(DataManager::Get().getReward(dis(gen)));
			ui.rewardBtn[i].setText(currentRewards[i].getName());
			ui.rewardBtn[i].setExplain(currentRewards[i].getDescription());
		}
		DataManager::Get().refreshTimesAdvance();
		if(player.getMoney()<DataManager::Get().getRefreshMoney()){
			ui.refreshBtn.setAvailibility(false);
		}else{
			ui.refreshBtn.setAvailibility(true);
		}
		ui.refreshBtn.setAddition(L"(-"+std::to_wstring(DataManager::Get().getRefreshMoney())+L")");
	}
}
void VictoryLevel::chooseReward(const int i, Player& player){
	TraceLog(LOG_INFO,"Choosing reward[%d]",i);
	DataManager::Get().resetRefreshTimes();
	currentRewards[i].apply(player);
}