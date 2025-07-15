#ifndef SHOPUI_H
#define SHOPUI_H
#include "BaseUI.h"
#include "Level/ShopLevel.h"
#include "UI/Button.h"
#include <vector>
class ShopUI:public BaseUI{
private:
	ShopUI();
	~ShopUI()=default;
	const bool isSkipButtonPressed()const;
	const bool isGoodButtonPressed(const int i)const;
	const bool isRefreshButtonPressed()const;
	void adjustButtonPosition();
	void setButtonAvailability();
	void attachGoodsCost();
	void disableGoods(const int i);
public:
	static ShopUI& Get(){
		static ShopUI instance;
		return instance;
	}
	void Draw()const override;
	void setup();
private:
	ShopLevel* shopPtr=nullptr;
	std::vector<GoodsButton> goodsBtn;
	ButtonWithNumber refreshBtn;
	Button skipBtn;
	Texture shopKeeperTexture;
	friend class ShopLevel;
};
#endif