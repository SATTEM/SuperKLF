#include "Level/ShopLevel.h"
#include "DataManager.h"
#include "EntityModifier.h"
#include "Level/LevelManager.h"
#include "UI/Button.h"
#include "UI/LevelUI/ShopUI.h"
#include "UI/UI.h"
#include <random>
#include <raylib.h>
void ShopLevel::onActivate(){
	Level::onActivate();
	ShopUI::Get().shopPtr=this;
	genGoods();
}
void ShopLevel::update(){
	ShopUI& ui=ShopUI::Get();
	DataManager& data=DataManager::Get();
	Player& player=ctrl.getPlayer();
	ui.Draw();
	if(ui.isRefreshButtonPressed()){
		bool success=(player.getMoney()-data.getRefreshMoney())>=0;
		if(success){
			EntityModifier::addMoney(player, -data.getRefreshMoney());
			genGoods();
		}
	}
	for(size_t i=0;i<currentRewards.size();i++){
		if(ui.isGoodButtonPressed(i)){
			buyGoods(i);
			LevelManager::Get().toNextBattle();
		}
	}
	if(ui.isSkipButtonPressed()){
		LevelManager::Get().toNextBattle();
	}
}
void ShopLevel::genGoods(){
//生成商品并绑定到UI
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis(0,DataManager::Get().getRewardSize()-1);
	ShopUI& ui=ShopUI::Get();
	DataManager& data=DataManager::Get();
	currentRewards.clear();
	rewardsCost.clear();
	ui.goodsBtn.clear();
	for(int i=0;i<data.getShopGoodSize();i++){
		currentRewards.push_back(data.getReward(dis(gen)));
		rewardsCost.push_back(currentRewards[i].getCost());
		ui.goodsBtn.push_back(GoodsButton({
			0,0,
			UI::ButtonCFG::BASIC_BUTTON_WIDTH,UI::ButtonCFG::BASIC_BUTTON_HEIGHT
		},L"",ORANGE,L""));
		ui.goodsBtn[i].setExplain(currentRewards[i].getDescription());
		ui.goodsBtn[i].setText(currentRewards[i].getName());
	}
	ui.setup();
	Player& player=ctrl.getPlayer();
	data.refreshTimesAdvance();
	if(player.getMoney()<data.getRefreshMoney()){
		ui.refreshBtn.setAvailibility(false);
	}else{
		ui.refreshBtn.setAvailibility(true);
	}
	ui.refreshBtn.setAddition(L"(-"+std::to_wstring(data.getRefreshMoney())+L")");
}
void ShopLevel::buyGoods(const int i){
//购买商品
	TraceLog(LOG_INFO, "Choosing goods %d",i);
	EntityModifier::addMoney(ctrl.getPlayer(), -rewardsCost[i]);
	DataManager::Get().resetRefreshTimes();
	currentRewards[i].apply(ctrl.getPlayer());
}