#ifndef BUTTON_H
#define BUTTON_H
extern "C"{
	#include "raylib.h"
}
#include<string>
#include "UIwheel.h"
class Button{
protected:
	bool available=true;
	Rectangle rect;
	std::wstring text;
	Color color;
	int fontSize;
	Vector2 textPos;
	void resetPosAndSize();
	virtual const std::wstring getFullText()const;
public:
	virtual void Draw() const;
	const Rectangle& getRect() const{return rect;}
	void setAvailibility(const bool a){available=a;}
	void setText(const std::wstring& str){text=str;resetPosAndSize();}
	std::wstring getText()const{return text;}
	Button()=default;
	Button(Rectangle r,std::wstring t,Color c);
	const bool isHoovered()const;
	const bool isPressed()const;
};
class ButtonWithNumber:public Button{
private:
	std::wstring addition;
	const std::wstring getFullText()const override;
public:
	ButtonWithNumber()=default;
	ButtonWithNumber(Rectangle r,std::wstring t,Color c,std::wstring a=L"")
	:Button(r,t,c),addition(a){}
	void setAddition(const std::wstring& str);
	void Draw() const override;
};
class ButtonWithExplain:public Button{
private:
	Detail explain;
	const std::wstring getFullText()const override;
public:
	ButtonWithExplain()=default;
	ButtonWithExplain(Rectangle r,std::wstring t,Color c,std::wstring e):Button(r,t,c),explain(e,c,WHITE,UI::ButtonCFG::EXPLAIN_LINE_MAX_CHAR){
		explain.setRectangle({rect.x,rect.y+rect.height,rect.width,rect.height*2.5f});
	}
	void setExplain(const std::wstring& str);
	void Draw() const override;
};

#endif