#include "UI/Button.h"
#include "Collision.h"
#include "ResourceManager.h"
#include "UI/UIUtility.h"
Button::Button(Rectangle r, std::string t, Color c) {
    rect = {r.x-r.width/2.f,r.y-r.height/2.f,r.width,r.height};
	if(rect.x<0){rect.x=0;}
	if(rect.y<0){rect.y=0;}
    text = t;
    color = c;
	resetPosAndSize();
}
const bool Button::isHoovered()const{
	return Collision::checkIsTouchButton(*this);
}
const bool Button::isPressed()const{
	return  isHoovered()&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void Button::resetPosAndSize(){
    const float maxTextWidth = rect.width * UI::WIDTH_RADIO;
    const float maxTextHeight = rect.height * UI::HEIGHT_RADIO;
	const Font& font=ResourceManager::Get().loadFont();
    int low = UI::MIN_FONT_SIZE;
    int high = int(maxTextHeight);
    int bestSize = UI::MIN_FONT_SIZE;
    while (low <= high) {
        int mid = (low + high) / 2;
        Vector2 textSize = MeasureTextEx(font, text.c_str(), mid, 1);
        if (textSize.x <= maxTextWidth) {
            bestSize = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    fontSize = bestSize;
	Vector2 textSize=MeasureTextEx(font, text.c_str(), fontSize, 1);
    textPos = {
		UI::countTextPosX(text, rect.x, fontSize,(rect.width/2)),
        rect.y + (rect.height -textSize.y)/2
    };	
}

void Button::Draw() const{
	Color drawColor =color;
	const Font& font=ResourceManager::Get().loadFont();
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
	DrawTextEx(font, text.c_str(), textPos, fontSize, 1, WHITE);
	EndScissorMode();
}
void ButtonWithExplain::Draw() const{
	Button::Draw();
	if(isHoovered()&&available){
        DrawExplain();
	}  
}
void ButtonWithExplain::DrawExplain() const{
    Rectangle explainBox={
        rect.x,rect.y+rect.height,rect.width,rect.height*2.5f
    };
    DrawRectangleRec(explainBox, ColorBrightness(color, 0.15f));
    Vector2 position={explainBox.x,explainBox.y};
    DrawTextEx(ResourceManager::Get().loadFont(), explain.c_str(), position, UI::EXPLAIN_FONTSIZE, 1, WHITE);
}

void ButtonWithNumber::Draw() const{
	Color drawColor =color;
	const Font& font=ResourceManager::Get().loadFont();
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
	DrawTextEx(font, (text+addition).c_str(), textPos, fontSize, 1, WHITE);
	EndScissorMode();
}
