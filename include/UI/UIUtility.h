#ifndef UIUTILITY_H
#define UIUTILITY_H
#include <string>
#include <nlohmann/json.hpp>
#include <functional>
extern "C"{
	#include "raylib.h"
}

namespace UI{
	enum class Align{Center,Left};
	const float countTextPosX(const std::string& str,const int origin,const int fontSize,const Font& font);
	const Vector2 countRelicDisplayPos(int i);
	//绘制文本
	void drawText(const std::string& text,const float posX,const float posY,const int fontSize,const Color& color,const Align& al=Align::Center);
	void drawText(const std::wstring& text,const float posX,const float posY,const int fontSize,const Color& color,const Align& al=Align::Center);
	void drawText(const std::string& text,const Vector2& pos,const int fontSize,const Color& color,const Align& al=Align::Center);
	//将文本限制在矩形内部，左对齐
	void drawTextInRect(const std::string& text, const Rectangle& rect, const int fontSize, const Color& color);
	void drawTextInRect(const std::wstring &text, const Rectangle &rect, const int fontSize, const Color &color);
	void drawTextInRectScrissored(const std::wstring &text, const Rectangle &rect, const int fontSize, const Color &color);
	void drawTextInRectScrissored(const std::string &text, const Rectangle &rect, const int fontSize, const Color &color);
	std::wstring autoSplit(const std::wstring&,const int);
	void drawInTop(const std::function<void()>&);
	void drawTop();
}


#endif