#ifndef BATTLEUI_H
#define BATTLEUI_H
#include "BaseUI.h"
#include "../Information.h"
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

#endif