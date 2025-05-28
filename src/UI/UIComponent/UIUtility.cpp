#include "UI/UIUtility.h"
#include "ResourceManager.h"
#include <vector>
#include <string>
#include "Tools.h"
extern "C"{
	#include "raylib.h"
}
void drawTextInRectHelper(const std::string& text,Vector2& holdSpace,const Rectangle& rect,const int& fontSize,const Color& color,Font& font);
const float UI::countTextPosX(const std::string& str,const int origin,const int fontSize,const Font& font){
	Vector2 textSize = MeasureTextEx(font, str.c_str(), fontSize, 1);
	float posX=origin-textSize.x/2.f;
	return posX;
}	
const Vector2 UI::countRelicDisplayPos(int i) {
    const float x = GetScreenWidth() - UI::RelicCFG::RELIC_DISPLAY_OFFSET - UI::RelicCFG::RELIC_TEXTURE_SIZE.x; // 距离屏幕右边的固定偏移量
    const float relicWidth = UI::RelicCFG::RELIC_TEXTURE_SIZE.x;
    const float relicHeight = UI::RelicCFG::RELIC_TEXTURE_SIZE.y;
    const float padding = UI::RelicCFG::RELIC_DISPLAY_PADDING;

    const float screenHeight = GetScreenHeight();
    const float totalHeight = (relicHeight + padding) * 3; //每列最多显示 3 个 relic
    const float offsetY = (screenHeight - totalHeight) / 2.0f;

    float y = offsetY + (i * (relicHeight + padding));
    return {x, y};
}
void UI::drawText(const std::string& text,const float posX,const float posY,const int fontSize,const Color& color,const Align& al){
	Font font=ResourceManager::Get().getFont(text);
    if(al==Align::Center){
        DrawTextEx(font,text.c_str(),
    {countTextPosX(text,posX,fontSize,font),posY},
	    fontSize,1,color);
    }else if(al==Align::Left){
        DrawTextEx(font, text.c_str(), {posX,posY}, fontSize, 1, color);
    }
}
void UI::drawText(const std::wstring& text,const float posX,const float posY,const int fontSize,const Color& color,const Align& al){
	drawText(Trans::wstrToUTF(text),posX,posY,fontSize,color,al);
}
void UI::drawText(const std::string &text, const Vector2 &pos, const int fontSize, const Color &color,const Align& al){
    drawText(text, pos.x, pos.y, fontSize, color,al);
}
void UI::drawTextInRect(const std::wstring& text, const Rectangle& rect, const int fontSize, const Color &color){
    drawTextInRect(Trans::wstrToUTF(text), rect, fontSize, color);
}
void UI::drawTextInRect(const std::string &text, const Rectangle &rect, const int fontSize, const Color &color){
    Font font = ResourceManager::Get().getFont(text);
    Vector2 holdSpace = MeasureTextEx(font, text.c_str(), fontSize, 1);
    drawTextInRectHelper(text, holdSpace, rect, fontSize, color, font);
}
void UI::drawTextInRectScrissored(const std::string &text, const Rectangle &rect, const int fontSize, const Color &color){
    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
    drawTextInRect(text,rect,fontSize,color);
    EndScissorMode();
}
void UI::drawTextInRectScrissored(const std::wstring &text, const Rectangle &rect, const int fontSize, const Color &color){
    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
    drawTextInRect(text,rect,fontSize,color);
    EndScissorMode();
}
void drawTextInRectHelper(const std::string& text,Vector2& holdSpace,const Rectangle& rect,const int& fontSize,const Color& color,Font& font){
    if(holdSpace.x <= rect.width){
        //无需多行显示的情况
        UI::drawText(text, {rect.x, rect.y}, fontSize, color, UI::Align::Left);
    } else {
        //需要多行显示的情况
        std::wstring wtext = Trans::UTFTowstr(text);
        //计算每行能容纳的字符数（除法不能得到正解）
        int maxCharsPerLine = 0;
        std::wstring testStr;
        for(size_t i = 0; i < wtext.length(); i++) {
            testStr += wtext[i];
            //逼近边界
            Vector2 testSize = MeasureTextEx(font, Trans::wstrToUTF(testStr).c_str(), fontSize, 1);
            if(testSize.x > rect.width) {
                maxCharsPerLine = i;
                break;
            }
        }
        if(maxCharsPerLine == 0) maxCharsPerLine = wtext.length();
        //以换行符作为绘制依据，因此要多添加末尾的\n
        std::wstring proText = UI::autoSplit(wtext, maxCharsPerLine)+L'\n';
        int row = 0;
        std::wstring line;
        for(size_t i = 0; i < proText.size(); i++){
            if(proText[i] != L'\n'){
                line += proText[i];
            } else {
                std::string utf8Line = Trans::wstrToUTF(line);
                UI::drawText(utf8Line, rect.x, rect.y + row * fontSize, fontSize, color, UI::Align::Left);
                row++;
                line.clear();
            }
        }
    }
}
void UI::drawInTop(const std::function<void()>& callback){
	UI::UIControl::allDrawCalls.push_back(callback);
}
void UI::drawTop(){
	for(const auto& draw:UI::UIControl::allDrawCalls){
		draw();
	}
	UI::UIControl::allDrawCalls.clear();
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