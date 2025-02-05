#include <memory>
#include <string>
extern "C"{
	#include "raylib.h"
}
#include "Bullet.h"
#include "Entity.h"
#include "Collision.h"
#include "ResourceManager.h"

const bool isOutOfScreen(const Vector2& pos);

Bullet::Bullet(const std::string texPath,const Vector2& vel,const Vector2& pos,const int dmg,const bool act)
	:position(pos),velocity(vel),active(act),colliderRadius(BULLET::BULLET_RADIUS),damage(dmg){
		Texture2D origin=ResourceManager::Get().loadTexture(texPath);
		ResourceManager::Get().resizeTexture(texPath, BULLET::BULLET_WIDTH, BULLET::BULLET_HEIGHT);
		texture=ResourceManager::Get().loadTexture(texPath);
	}

std::unique_ptr<Bullet> Bullet::shoot(const Vector2& begin){
	if(active==false){
		//子弹原型时（只有原型会调用这个函数）
		return std::unique_ptr<Bullet>{new Bullet(texture,velocity,begin,true)};
	}else{
		return nullptr;
	}
}
void Bullet::Draw() const { 
	if(IsTextureValid(texture)){
		DrawTextureV(texture, position, WHITE); 
		DrawCircleLinesV(getColliderCentre(),colliderRadius,BLUE);
	}
	
}
void Bullet::Update(const float deltaTime,Entity& aEntity){
	if(active){
		//只有克隆体是“实例”
		position.x+=velocity.x*deltaTime*BULLET::BASIC_SPEED;
		position.y+=velocity.y*deltaTime*BULLET::BASIC_SPEED;
		Draw();
		checkHit(aEntity);
	}
}
void Bullet::checkHit(Entity& aEntity) {
	//判断是否碰到目标，如果碰到了，发送伤害，并返回true，否则返回false
	if(Collision::checkBulletEntity(*this, aEntity)){
		aEntity.takeDamage(damage);
		shouldRemove=true;
	}else{
		if(isOutOfScreen(this->getColliderCentre())){
			shouldRemove=true;
		}
	}
}

const bool isOutOfScreen(const Vector2& pos){
	if(pos.x>GetScreenWidth()||pos.x<0){
		return true;
	}
	if(pos.y>GetScreenWidth()||pos.y<0){
		return true;
	}
	return false;
}