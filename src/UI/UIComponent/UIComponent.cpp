#include "UIComponent.h"
#include "ResourceManager.h"
#include "Collision.h"
#include "UI.h"
#include "GameStage.h"
extern "C"{
    #include "raylib.h"
}
const bool isButtonPressed(const Button& btn);
const bool isButtonHovered(const Button& btn);

const float UI::countTextPosX(const std::string& str,const int origin,const int fontSize,const int offset){
	float posX=origin;
	const Font& font=ResourceManager::Get().loadFont();
	posX-=MeasureTextEx(font, str.c_str(), fontSize, 1).x/2-offset;
	return posX;
}	

void BulletDisplay::Draw() const{
	Vector2 nowPos={box.x+UI::BULLET_DISPLAY_OFFSET,box.y+UI::BULLET_DISPLAY_OFFSET};
	DrawRectangle(box.x,box.y,box.width,box.height,Fade(GOLD,0.4));
	for(auto& bullet:StageController::Get().getPlayerBulletPattern()){
		bullet.DrawAsPattern(nowPos,bulletScale);
		nowPos={nowPos.x+BULLET::BULLET_SIZE.x,nowPos.y};
		if(nowPos.x>box.width){
			nowPos.x=box.x+UI::BULLET_DISPLAY_OFFSET;
			nowPos.y+=BULLET::BULLET_SIZE.y;
		}
	}
}

Button::Button(Rectangle r, std::string t, Color c) {
    rect = r;
    text = t;
    color = c;
	resetPosAndSize();
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
		if(isButtonPressed(*this)){
			drawColor=ColorBrightness(color, -0.3f);
		}else if(isButtonHovered(*this)){
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
	if(isButtonHovered(*this)&&available){
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
		if(isButtonPressed(*this)){
			drawColor=ColorBrightness(color, -0.3f);
		}else if(isButtonHovered(*this)){
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

const bool isButtonPressed(const Button& btn){
	return isButtonHovered(btn)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
const bool isButtonHovered(const Button& btn){
	return Collision::checkIsTouchButton(btn);
}