#include "UI/UIUtility.h"
#include "ResourceManager.h"
#include <codecvt>
#include <vector>
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
const Vector2 UI::countRelicDisplayPos(int i) {
    const float x = GetScreenWidth() - UI::RELIC_DISPLAY_OFFSET - UI::RELIC_TEXTURE_SIZE.x; // 距离屏幕右边的固定偏移量
    const float relicWidth = UI::RELIC_TEXTURE_SIZE.x;
    const float relicHeight = UI::RELIC_TEXTURE_SIZE.y;
    const float padding = UI::RELIC_DISPLAY_PADDING;

    const float screenHeight = GetScreenHeight();
    const float totalHeight = (relicHeight + padding) * 3; //每列最多显示 3 个 relic
    const float offsetY = (screenHeight - totalHeight) / 2.0f;

    float y = offsetY + (i * (relicHeight + padding));
    return {x, y};
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

void UI::drawInTop(const std::function<void()>& callback){
	UI::allDrawCalls.push_back(callback);
}
void UI::drawTop(){
	for(const auto& draw:UI::allDrawCalls){
		draw();
	}
	UI::allDrawCalls.clear();
}

std::wstring UI::autoSplit(const std::wstring& str,const int lineMax){
    if(lineMax <= 0) return str;
    std::wstring result;
    result.reserve(str.size() + str.size()/lineMax); // 预分配
    int charInline = 0;
    for(auto c : str) {
        result += c;
        charInline = (c == L'\n') ? 0 : charInline + 1;
        if(charInline >= lineMax) {
            result += L'\n';
            charInline = 0;
        }
    }
    return result;
}