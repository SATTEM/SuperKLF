#include "UI/Information.h"
#include "UI/UIUtility.h"
#include "GameStage.h"
void BulletDisplay::Draw() const{
	Vector2 nowPos={box.x+UI::BULLET_DISPLAY_OFFSET,box.y+UI::BULLET_DISPLAY_OFFSET};
	DrawRectangle(box.x,box.y,box.width,box.height,Fade(GRAY,0.4));
	DrawRectangleLines(box.x, box.y, box.width, box.height, YELLOW);
	for(auto& bullet:StageController::Get().getPlayerBulletPattern()){
		bullet.DrawAsPattern(nowPos,bulletScale);
		nowPos={nowPos.x+BULLET::BULLET_SIZE.x,nowPos.y};
		if(nowPos.x>box.width){
			nowPos.x=box.x+UI::BULLET_DISPLAY_OFFSET;
			nowPos.y+=BULLET::BULLET_SIZE.y;
		}
	}
}
