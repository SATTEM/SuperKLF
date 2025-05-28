#ifndef UIUTILITY_H
#define UIUTILITY_H
#include <string>
#include <nlohmann/json.hpp>
#include <functional>
extern "C"{
	#include "raylib.h"
}
namespace UI {
	namespace UIControl{
		static std::vector<std::function<void()>> allDrawCalls;
	}
	namespace EventCFG{
		const int EVENT_FONT_SIZE=25;	
		const Rectangle EVENT_SCALE={0.04f,0.2f,0.92f,0.5f};
	}
	namespace FontCFG{
		const int MIN_FONT_SIZE=1;
		const int FONTSIZE=40;
		const int FONT_LOAD_SIZE=200;
	}
		
	namespace BulletCFG{
		const int BULLET_DISPLAY_OFFSET=2;	
		const int BULLET_DISPLAY_HEIGHT=240;
		const int BULLET_DISPLAY_WIDTH=120;	
		const float BULLET_DISPLAY_SCALE=0.15;		
	}
	namespace ButtonCFG{
		const int BASIC_BUTTON_WIDTH = 200;
		const int BASIC_BUTTON_HEIGHT = 50;	
		const float WIDTH_RADIO=0.9f;
		const float HEIGHT_RADIO=0.5f;	
		const int EXPLAIN_LINE_MAX_CHAR=26;	
		const int EXPLAIN_FONTSIZE=20;
	}
	//Relic
	namespace RelicCFG{
		const Vector2 RELIC_TEXTURE_SIZE={32,32};
		const float RELIC_DISPLAY_PADDING=3.f;
		const float RELIC_DISPLAY_OFFSET=20.f;	
		const int RELIC_DISPLAY_MAX_LENGTH=4;
	}
	namespace EntityCFG{
		static Vector2 DEFAULT_ENEMY_POSITION={640,100};
		static Vector2 DEFAULT_PLAYER_POSTION={640,350};		
	}
	//Entity

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