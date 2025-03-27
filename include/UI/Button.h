#ifndef BUTTON_H
#define BUTTON_H
extern "C"{
	#include "raylib.h"
}
#include<string>

class Button{
protected:
	bool available=true;
	Rectangle rect;
	std::string text;
	Color color;
	int fontSize;
	Vector2 textPos;
	void resetPosAndSize();
public:
	virtual void Draw() const;
	const Rectangle& getRect() const{return rect;}
	void setAvailibility(const bool a){available=a;}
	void setText(const std::string& str){text=str;resetPosAndSize();}
	std::string getText()const{return text;}
	Button()=default;
	Button(Rectangle r,std::string t,Color c);
	const bool isHoovered()const;
	const bool isPressed()const;
};
class ButtonWithNumber:public Button{
private:
	std::string addition;
public:
	ButtonWithNumber()=default;
	ButtonWithNumber(Rectangle r,std::string t,Color c,std::string a="")
	:Button(r,t,c),addition(a){}
	void setAddition(const std::string& str){addition=str;}
	void Draw() const override;
};
class ButtonWithExplain:public Button{
private:
	std::string explain;
	void DrawExplain() const;
public:
	ButtonWithExplain()=default;
	ButtonWithExplain(Rectangle r,std::string t,Color c,std::string e):Button(r,t,c),explain(e){}
	void setExplain(const std::string& str){explain=str;}
	void Draw() const override;
};

#endif