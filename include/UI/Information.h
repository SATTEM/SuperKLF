#ifndef INFORMATION_H
#define INFORMATION_H
#include "UI/UIwheel.h"
extern "C"{
	#include "raylib.h"
}
class BulletDisplay{
private:
	Rectangle box;
	float bulletScale=UI::BulletCFG::BULLET_DISPLAY_SCALE;
public:
	BulletDisplay(const Rectangle& b):box(b){}
	~BulletDisplay()=default;
	void Draw()const;
};
class TextureDetailedDisplay{
private:
	Vector2 position;
	Rectangle rect;
	Texture2D texture;
	Detail detail;
	int length;
public:
	TextureDetailedDisplay(const Texture2D&,const std::wstring&,const Vector2&,const int lineLength,const int fontSize=UI::ButtonCFG::EXPLAIN_FONTSIZE);
	~TextureDetailedDisplay()=default;
	void setPosition(const Vector2 pos){position=pos;rect={pos.x,pos.y,float(texture.width),float(texture.height)};}
	void Draw()const;

};

#endif