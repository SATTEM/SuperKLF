#include "UI/UIUtility.h"
#include "ResourceManager.h"
#include <codecvt>
#include <locale>
#include <string>
extern "C"{
	#include "raylib.h"
}
namespace UI{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
}
const float UI::countTextPosX(const std::string& str,const int origin,const int fontSize,const Font& font){
	Vector2 textSize = MeasureTextEx(font, str.c_str(), fontSize, 1);
	float posX=origin-textSize.x/2.f;
	return posX;
}	
std::string UI::wstrToUTF(const std::wstring & text){
	return converter.to_bytes(text);
} 
std::wstring UI::UTFTowstr(const std::string & text){
	return converter.from_bytes(text);
}
void UI::drawText(const std::string& text,const float posX,const float posY,const int fontSize,const Color& color){
	Font font=ResourceManager::Get().getFont(text);
	DrawTextEx(font,text.c_str(),{countTextPosX(text,posX,fontSize,font),posY},
	fontSize,0,color);
}
void UI::drawText(const std::wstring& text,const float posX,const float posY,const int fontSize,const Color& color){
	drawText(wstrToUTF(text),posX,posY,fontSize,color);
}