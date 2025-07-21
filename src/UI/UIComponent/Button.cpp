#include "UI/Button.h"
#include "Tools.h"
#include "ResourceManager.h"
#include "UI/UIUtility.h"
Button::Button(const Rectangle& r,const std::wstring& t,const Color& c) {
    rect = {r.x-r.width/2.f,r.y-r.height/2.f,r.width,r.height};
	if(rect.x<0){rect.x=0;}
	if(rect.y<0){rect.y=0;}
    text = t;
    color = c;
	resetPosAndSize();
}
const bool Button::isHoovered()const{
	return Collision::checkMouseTouch(rect);
}
const bool Button::isPressed()const{
	if(available==false){return false;}
	return  isHoovered()&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void Button::resetPosAndSize(){
    const float maxTextWidth = rect.width * UI::ButtonCFG::WIDTH_RADIO;
    const float maxTextHeight = rect.height * UI::ButtonCFG::HEIGHT_RADIO;
	std::wstring fullTextW=getFullText();
	std::string fullText=Trans::wstrToUTF(fullTextW);
	const Font& font=ResourceManager::Get().getFont(fullText);
    int low = UI::FontCFG::MIN_FONT_SIZE;
    int high = int(maxTextHeight);
    int bestSize = UI::FontCFG::MIN_FONT_SIZE;
    while (low <= high) {
        int mid = (low + high) / 2;
        Vector2 textSize = MeasureTextEx(font, fullText.c_str(), mid, 1);
        if (textSize.x <= maxTextWidth) {
            bestSize = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    fontSize = bestSize;
	Vector2 textSize=MeasureTextEx(font, fullText.c_str(), fontSize, 1);
    textPos = {
		rect.x + rect.width/2.f,
        rect.y + (rect.height -textSize.y)/2
    };	
}
const std::wstring Button::getFullText()const{
	return text;
}

void Button::Draw() const{
	Color drawColor =color;
	if(available){
		if(isPressed()){
			drawColor=ColorBrightness(color, -0.3f);
		}else if(isHoovered()){
			drawColor=ColorBrightness(color, -0.15f);
		}
	}else{
		drawColor=DARKGRAY;
	}
	DrawRectangleRec(rect, drawColor);
	BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
	UI::drawText(text,textPos.x,textPos.y,fontSize,WHITE);
	EndScissorMode();
}
void ButtonWithExplain::setExplain(const std::wstring& str){
	if(explain==str){return;}
	explain=str;
}
const std::wstring ButtonWithExplain::getFullText()const{
	return text;
}
void ButtonWithExplain::Draw() const{
	Button::Draw();
	if(isHoovered()&&available){
        explain.draw();
	}  
}

void ButtonWithNumber::Draw() const{
	Color drawColor =color;
	const Font& font=ResourceManager::Get().getFont(Trans::wstrToUTF(text+addition));
	if(available){
		if(isPressed()){
			drawColor=ColorBrightness(color, -0.3f);
		}else if(isHoovered()){
			drawColor=ColorBrightness(color, -0.15f);
		}
	}else{
		drawColor=DARKGRAY;
	}
	DrawRectangleRec(rect, drawColor);
	UI::drawText(text+addition,textPos.x,textPos.y,fontSize,WHITE);
}

void ButtonWithNumber::setAddition(const std::wstring& str){
	if(str==addition){
		return;
	}
	addition=str;
	resetPosAndSize();
}
const std::wstring ButtonWithNumber::getFullText()const{
	return text+addition;
}