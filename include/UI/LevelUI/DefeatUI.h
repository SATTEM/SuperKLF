#ifndef DEFEATUI_H
#define DEFEATUI_H
#include "BaseUI.h"
#include "../Button.h"
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

#endif