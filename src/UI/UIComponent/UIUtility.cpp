#include "UI/UIUtility.h"
#include "ResourceManager.h"
const float UI::countTextPosX(const std::string& str,const int origin,const int fontSize,const int offset){
	float posX=origin;
	const Font& font=ResourceManager::Get().loadFont();
	posX-=MeasureTextEx(font, str.c_str(), fontSize, 1).x/2-offset;
	return posX;
}	