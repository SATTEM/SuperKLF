#include "UI/UIUtility.h"
#include "ResourceManager.h"
const float UI::countTextPosX(const std::string& str,const int origin,const int fontSize,const int offset){
	float posX=origin;
	const Font& font=ResourceManager::Get().loadFont();
	posX-=MeasureTextEx(font, str.c_str(), fontSize, 1).x/2-offset;
	return posX;
}	
void UI::drawText(const std::string& text,const float posX,const float posY,const int fontSize,const Color& color){
	DrawTextEx(ResourceManager::Get().loadFont(),
	text.c_str(),{countTextPosX(text,posX,fontSize),posY},
	fontSize,0,color);
}