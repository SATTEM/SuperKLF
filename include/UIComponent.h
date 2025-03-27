#ifndef UICOMPONENT_H
#define UICOMPONENT_H
#include <string>
extern "C"{
	#include "raylib.h"
}
namespace UI{
	const int BASIC_BUTTON_WIDTH = 200;
	const int BASIC_BUTTON_HEIGHT = 50;
	const float WIDTH_RADIO=0.9f;
	const float HEIGHT_RADIO=0.5f;
	const float countTextPosX(const std::string& str,const int origin,const int fontSize,const int offset=0);
}

class BulletDisplay{
private:
	Rectangle box;
	float bulletScale=0.f;
public:
	BulletDisplay(const Rectangle& b):box(b){}
	~BulletDisplay()=default;
	void setBulletScale(const float scale){bulletScale=scale;}
	void Draw()const;
};

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