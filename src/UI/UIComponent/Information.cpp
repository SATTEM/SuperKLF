#include "UI/Information.h"
#include "UI/UIUtility.h"
#include "GameStage.h"
#include "Tools.h"
void BulletDisplay::Draw() const{
	Vector2 nowPos={box.x+UI::BulletCFG::BULLET_DISPLAY_OFFSET,box.y+UI::BulletCFG::BULLET_DISPLAY_OFFSET};
	DrawRectangle(box.x,box.y,box.width,box.height,Fade(GRAY,0.4));
	DrawRectangleLines(box.x, box.y, box.width, box.height, YELLOW);
	for(auto& bullet:StageController::Get().getPlayerBulletPattern()){
		bullet.DrawAsPattern(nowPos,bulletScale);
		nowPos={nowPos.x+BULLET::BULLET_SIZE.x,nowPos.y};
		if(nowPos.x>box.width){
			nowPos.x=box.x+UI::BulletCFG::BULLET_DISPLAY_OFFSET;
			nowPos.y+=BULLET::BULLET_SIZE.y;
		}
	}
}

TextureDetailedDisplay::TextureDetailedDisplay(const Texture2D& tex,const std::wstring& text,const Vector2& pos,const int lineLength,const int fontSize)
:texture(tex),detail(text),position(pos){
	if(!IsTextureValid(texture)){
		TraceLog(LOG_ERROR,"[Display] invalid texture");
	}
	rect={position.x,position.y,float(texture.width),float(texture.height)};
	detail.setRectangle({position.x,position.y+texture.height,float(texture.width),float(texture.height)});
	detail.setFontSize(fontSize);
}

void TextureDetailedDisplay::Draw()const{
	DrawTextureV(texture,position,WHITE);
	if(Collision::checkMouseTouch(rect)){
		detail.draw();
	}
}
