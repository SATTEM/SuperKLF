#ifndef UIUTILITY_H
#define UIUTILITY_H
#include<string>
#include <nlohmann/json.hpp>
extern "C"{
	#include "raylib.h"
}
namespace UI{
	const float countTextPosX(const std::string& str,const int origin,const int fontSize,const Font& font);
	std::string wstrToUTF(const std::wstring&);
	std::wstring UTFTowstr(const std::string&);
	void drawText(const std::string& text,const float posX,const float posY,const int fontSize,const Color& color);
	void drawText(const std::wstring& text,const float posX,const float posY,const int fontSize,const Color& color);
}

namespace UI {
	//Font
	const int MIN_FONT_SIZE=1;	
	const int FONTSIZE=40;
	const int FONT_LOAD_SIZE=200;
	//Bullet
	const int BULLET_DISPLAY_OFFSET=2;	
	const int BULLET_DISPLAY_HEIGHT=240;
	const int BULLET_DISPLAY_WIDTH=120;
	const float BULLET_DISPLAY_SCALE=0.15;
	//Button
	const int BASIC_BUTTON_WIDTH = 200;
	const int BASIC_BUTTON_HEIGHT = 50;
	const float WIDTH_RADIO=0.9f;
	const float HEIGHT_RADIO=0.5f;
	const int EXPLAIN_LINE_MAX_CHAR=26;
	const int EXPLAIN_FONTSIZE=20;
}

#endif