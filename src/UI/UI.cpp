#include "UI.h"
#include "Collision.h"
#include <raylib.h>
bool isButtonPressed(const Button& btn);
bool isButtonHovered(const Button& btn);
Button::Button(Rectangle r, std::string t, Color c) {
    rect = r;
    text = t;
    color = c;
    const float maxTextWidth = rect.width * UI::WIDTH_RADIO;
    const float maxTextHeight = rect.height * UI::HEIGHT_RADIO;
    int low = UI::MIN_FONT_SIZE;
    int high = int(maxTextHeight);
    int bestSize = UI::MIN_FONT_SIZE;
    while (low <= high) {
        int mid = (low + high) / 2;
        float textSize = MeasureText( text.c_str(), mid);
        if (textSize <= maxTextWidth) {
            bestSize = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    fontSize = bestSize;
    float textSize = MeasureText( text.c_str(), fontSize);
    textPos = {
        rect.x + (rect.width - textSize) / 2,
        rect.y + (rect.height-maxTextHeight) / 2
    };
}
void Button::Draw() const{
	Color drawColor =color;
	if(isButtonPressed(*this)){
		drawColor=ColorBrightness(color, -0.3f);
	}else if(isButtonHovered(*this)){
		drawColor=ColorBrightness(color, -0.15f);
	}
	DrawRectangleRec(rect, drawColor);
	BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
	DrawTextEx(GetFontDefault(), text.c_str(), textPos, fontSize, 1, WHITE);
	EndScissorMode();
}
bool isButtonPressed(const Button& btn){
	return isButtonHovered(btn)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
bool isButtonHovered(const Button& btn){
	return Collision::checkIsTouchButton(btn);
}
void DefeatUI::Draw() const{
	ClearBackground(WHITE);	
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK,0.5f));
	DrawText("Mamba Out!",500,200,2*UI::FONTSIZE,RED);
	DrawText(TextFormat("You have passed %d levels!",passLevel), 500, 350, UI::FONTSIZE, RED);
	restartButton.Draw();
	exitButton.Draw();
}

DefeatUI::DefeatUI(){
	const int screenHeight=GetScreenHeight();
	const int screenWidth=GetScreenWidth();
	restartButton={
	{screenWidth/2.0f-UI::BASIC_BUTTON_WIDTH/2.0f,
		screenHeight*0.6f,
		UI::BASIC_BUTTON_WIDTH,
		UI::BASIC_BUTTON_HEIGHT},
	"[Space]Restart",ORANGE};
	exitButton={{
		screenWidth/2.0f-UI::BASIC_BUTTON_WIDTH/2.0f,
		screenHeight*0.75f,
		UI::BASIC_BUTTON_WIDTH,
		UI::BASIC_BUTTON_HEIGHT},
		"Quit",ORANGE};
}