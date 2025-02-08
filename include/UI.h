#ifndef UI_H
#define UI_H
#include <string>
extern "C"{
	#include "raylib.h"
}
namespace UI {
	const int BASIC_BUTTON_WIDTH = 200;
	const int BASIC_BUTTON_HEIGHT = 50;
	const float WIDTH_RADIO=0.9f;
	const float HEIGHT_RADIO=0.5f;
	const int MIN_FONT_SIZE=1;
	const int FONTSIZE=40;
}
class Button{
private:
	Rectangle rect;
	std::string text;
	Color color;
	int fontSize;
	Vector2 textPos;
public:
	void Draw() const;
	const Rectangle& getRect() const{return rect;}
	Button()=default;
	Button(Rectangle r,std::string t,Color c);
};
class DefeatUI{
private:
	Button restartButton,exitButton;
	int passLevel=0;
	DefeatUI();
	~DefeatUI()=default;
public:
	DefeatUI(const DefeatUI&)=delete;
	void operator=(const DefeatUI&)=delete;
	static DefeatUI& Get(){
		static DefeatUI instance;
		return instance;
	}
	void reset(){passLevel=0;}
	void advance(const int val){passLevel+=val;}
	const Button& getRestartBtn() const{return restartButton;}
	const Button& getExitBtn() const{return exitButton;}
	void Draw() const;
};

#endif