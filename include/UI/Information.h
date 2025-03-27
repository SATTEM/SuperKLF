#ifndef INFORMATION_H
#define INFORMATION_H
extern "C"{
	#include "raylib.h"
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

#endif