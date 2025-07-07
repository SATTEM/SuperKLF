#ifndef BUTTON_H
#define BUTTON_H
#include "UI/UI.h"
extern "C"{
	#include "raylib.h"
}
#include<string>
#include "UIwheel.h"
class Button{
protected:
	void resetPosAndSize();
	virtual const std::wstring getFullText()const;
public:
	Button()=default;
	Button(const Rectangle& r,const std::wstring& t,const Color& c);
	void setAvailibility(const bool a){available=a;}
	void setText(const std::wstring& str){text=str;resetPosAndSize();}
	const std::wstring& getText()const{return text;}
	const Rectangle& getRect() const{return rect;}
	const bool isHoovered()const;
	const bool isPressed()const;
	virtual void Draw() const;	
	virtual void setPosition(const Vector2& pos){
		rect={pos.x,pos.y,rect.width,rect.height};
		resetPosAndSize();
	}
protected:
	bool available=true;
	Rectangle rect={0,0,UI::ButtonCFG::BASIC_BUTTON_WIDTH,UI::ButtonCFG::BASIC_BUTTON_HEIGHT};
	std::wstring text;
	Color color;
	int fontSize;
	Vector2 textPos;
};
class ButtonWithNumber:virtual public Button{
public:
	ButtonWithNumber()=default;
	ButtonWithNumber(const Rectangle& r,const std::wstring& t,const Color& c,const std::wstring& a=L"")
	:Button(r,t,c),addition(a){}
	//由于虚基类，需要声明：
	//拷贝赋值运算符
	ButtonWithNumber& operator=(ButtonWithNumber&)=default;
	//拷贝构造函数
	ButtonWithNumber(const ButtonWithNumber& other):Button(other),addition(other.addition){}	
	//因为需要显式处理移动赋值运算符
	ButtonWithNumber& operator=(ButtonWithNumber&& other){
		if(this!=&other){
			Button::operator=(other);
			addition=other.addition;
		}
		return *this;
	}
	void setAddition(const std::wstring& str);
	virtual void Draw() const override;
protected:
	std::wstring addition;
	virtual const std::wstring getFullText()const override;
};
class ButtonWithExplain:virtual public Button{
public:
	ButtonWithExplain()=default;
	ButtonWithExplain(const Rectangle& r,const std::wstring& t,const Color& c,const std::wstring& e):Button(r,t,c),explain(e,c,WHITE,UI::ButtonCFG::EXPLAIN_LINE_MAX_CHAR){
		explain.setRectangle({rect.x,rect.y+rect.height,rect.width,rect.height*UI::ButtonCFG::EXPLAIN_HEIGHT_FACTOR});
	}
	ButtonWithExplain(const ButtonWithExplain& other):Button(other),explain(other.explain){}
	ButtonWithExplain& operator=(ButtonWithExplain&)=default;
	ButtonWithExplain& operator=(ButtonWithExplain&& other){
		if(this!=&other){
			Button::operator=(other);
			explain=other.explain;
		}
		return *this;
	}
	void setExplain(const std::wstring& str);
	virtual void Draw() const override;
	virtual void setPosition(const Vector2& pos)override{
		explain.setPosition({pos.x,pos.y+rect.height});
		Button::setPosition(pos);
	}
private:
	Detail explain;
	virtual const std::wstring getFullText()const override;	
};
class GoodsButton:public ButtonWithNumber,public ButtonWithExplain{
public:
	GoodsButton()=default;
	GoodsButton(const Rectangle& r,const std::wstring& t,const Color& c,const std::wstring& e,const std::wstring& a=L"")
	:Button(r,t,c),ButtonWithNumber(r,t,c,a),ButtonWithExplain(r, t, c, e){}
	GoodsButton(const GoodsButton& other):Button(other),ButtonWithNumber(other),ButtonWithExplain(other){}
	virtual const std::wstring getFullText()const override{
		return ButtonWithNumber::getFullText();
	}
	virtual void setPosition(const Vector2& pos)override{
		ButtonWithExplain::setPosition(pos);
	}
	virtual void Draw()const override{
		ButtonWithExplain::Draw();
		ButtonWithNumber::Draw();
	}
};
#endif