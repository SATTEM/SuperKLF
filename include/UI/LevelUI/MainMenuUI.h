#ifndef MAINMENUUI_H
#define MAINMENUUI_H
#include "BaseUI.h"
#include "../Button.h"
class MainMenuUI:public BaseUI{
private:
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
	const bool isLoad()const;
private:
	Button startButton,exitButton,continueButton;
};
#endif