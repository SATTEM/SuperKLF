#include "UI/LevelUI/ShopUI.h"
#include "DataManager.h"
#include "GameStage.h"
#include "Level/ShopLevel.h"
#include "ResourceManager.h"
#include "Tools.h"
#include "UI/LevelUI/BaseUI.h"
#include "UI/UI.h"
#include "UI/UIUtility.h"
#include <raylib.h>
#include <string>
using namespace UI::ShopCFG;
ShopUI::ShopUI(){
	const float screenHeight=GetScreenHeight();
	const float screenWidth=GetScreenWidth();
	shopKeeperTexture=ResourceManager::Get().loadTexture(ASSETS_IMAGE_PATH "shopkeeper.png");
	refreshBtn={{
		screenWidth*0.45f,
		screenHeight*0.9f,
		UI::ButtonCFG::BASIC_BUTTON_WIDTH*0.6f,
		UI::ButtonCFG::BASIC_BUTTON_HEIGHT
	},L"刷新",ORANGE};
	skipBtn={{
		screenWidth*0.55f,
		screenHeight*0.9f,
		UI::ButtonCFG::BASIC_BUTTON_WIDTH*0.6f,
		UI::ButtonCFG::BASIC_BUTTON_HEIGHT
	},L"跳过",ORANGE};
}
void ShopUI::setup(){
	if(shopPtr==nullptr){
		TraceLog(LOG_ERROR, "Calling ShopUI::setup() with nullptr!");
		return;
	}
	adjustButtonPosition();
	setButtonAvailability();
	attachGoodsCost();
}
void ShopUI::adjustButtonPosition(){
	//自动调整商品按钮的位置
	static Rectangle goodGrid={
		GetScreenWidth()*GOODS_GRID_FACTOR.x,
		GetScreenHeight()*GOODS_GRID_FACTOR.y,
		GetScreenWidth()*GOODS_GRID_FACTOR.width,
		GetScreenHeight()*GOODS_GRID_FACTOR.height
	};
	DataManager& data=DataManager::Get();
	Vector2 nextButtonPos={goodGrid.x,goodGrid.y};
	for(auto& btn:goodsBtn){
		if(nextButtonPos.x+btn.getRect().width*0.5>=goodGrid.x+goodGrid.width){
			nextButtonPos.x=goodGrid.x;
			nextButtonPos.y+=btn.getRect().height*(1+UI::ShopCFG::GOODS_GAP_FACTOR);
		}
		btn.setPosition(nextButtonPos);
		nextButtonPos.x+=btn.getRect().width*(1+UI::ShopCFG::GOODS_GAP_FACTOR);
	}
}
void ShopUI::setButtonAvailability(){
	int playerMoney=StageController::Get().getPlayer().getMoney();
	const auto& rewardsCost=shopPtr->rewardsCost;
	for(size_t i=0;i<goodsBtn.size();i++){
		if(playerMoney>=rewardsCost[i]){
			goodsBtn[i].setAvailibility(true);
		}else{
			goodsBtn[i].setAvailibility(false);
		}
	}
}
void ShopUI::attachGoodsCost(){
	const auto& rewardsCost=shopPtr->rewardsCost;
	for(size_t i=0;i<goodsBtn.size();i++){
		goodsBtn[i].setAddition(Trans::UTFTowstr(std::to_string(rewardsCost[i])));
	}
}
void ShopUI::Draw()const{
	ClearBackground(WHITE);
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.5f));
	UI::drawText("商店！购买你能买得起的东西吧!",
		screenWidth*0.5f,screenHeight*0.2f,2*UI::FontCFG::FONTSIZE,GOLD);
	DrawTextureV(shopKeeperTexture,{screenWidth*0.8f,screenHeight*0.45f},WHITE);
	for(const auto& btn:goodsBtn){
		btn.Draw();
	}
	refreshBtn.Draw();
	skipBtn.Draw();
	BaseUI::Draw();
}
const bool ShopUI::isRefreshButtonPressed()const{
	return refreshBtn.isPressed();
}
const bool ShopUI::isSkipButtonPressed()const{
	return skipBtn.isPressed();
}
const bool ShopUI::isGoodButtonPressed(const int i)const{
	return goodsBtn.at(i).isPressed();
}
void ShopUI::disableGoods(const int i){
	goodsBtn.at(i).setAvailibility(false);
}