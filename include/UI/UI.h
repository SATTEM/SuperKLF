#ifndef UI_H
#define UI_H
#include <nlohmann/json.hpp>
#include <functional>
extern "C"{
	#include "raylib.h"
}
namespace UI {
	namespace UIControl{
		static std::vector<std::function<void()>> allDrawCalls;

	}
	namespace UICFG{
		const int WindowWidth=1280;
		const int WindowHeight=720;		
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
		const float EXPLAIN_HEIGHT_FACTOR=2.5f;
		const int EXPLAIN_LINE_MAX_CHAR=26;	
		const int EXPLAIN_FONTSIZE=20;
	}
	namespace RelicCFG{
		const Vector2 RELIC_TEXTURE_SIZE={32,32};
		const float RELIC_DISPLAY_PADDING=3.f;
		const float RELIC_DISPLAY_OFFSET=20.f;	
		const int RELIC_DISPLAY_MAX_LENGTH=4;
	}
	namespace EntityCFG{
		const Vector2 DEFAULT_ENEMY_POSITION={0.5f,0.2f};
		const Vector2 DEFAULT_PLAYER_POSTION={0.5f,0.7f};		
	}
	namespace ShopCFG{
		const Rectangle GOODS_GRID_FACTOR={0.2f,0.45f,0.5f,0.3f};
		const float GOODS_GAP_FACTOR=0.05f;
	}
}
#endif