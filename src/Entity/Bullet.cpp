#include "Event/EventFWD.h"
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
extern "C"{
	#include "raylib.h"
}
#include "Bullet.h"
#include "Entity.h"
#include "Collision.h"
#include "ResourceManager.h"
#include "Event/EventSystem.h"

const bool isOutOfScreen(const Vector2& pos);

Bullet::Bullet(const std::string texPath,const Vector2& vel,const Vector2& pos,const int dmg,const bool act)
	:position(pos),velocity(vel),active(act),damage(dmg),texturePath(texPath){
		Texture2D origin=ResourceManager::Get().loadTexture(texPath);
		texturePath=ResourceManager::Get().resizeTexture(texPath, BULLET::BULLET_SIZE.x, BULLET::BULLET_SIZE.y);
		texture=ResourceManager::Get().loadTexture(texturePath);
		countColliderRadius();
		effects.push_back(std::make_shared<GiveDamage>(GiveDamage(damage)));
	}
void Bullet::countColliderRadius(const Vector2 size){
	colliderRadius=drawScale*(std::min(size.x,size.y)/2);
}

Bullet::Bullet(const Bullet& proto,const Vector2& begin)
	:position(begin),velocity(proto.velocity),active(true)
	,colliderRadius(proto.colliderRadius),damage(proto.damage)
	,texture(proto.texture),drawScale(proto.drawScale){
		effects=proto.effects;
	}

std::unique_ptr<Bullet> Bullet::shoot(const Vector2& begin){
	if(active==false){
		//子弹原型时（只有原型会调用这个函数）
		return std::make_unique<Bullet>(*this,begin);
	}else{
		return nullptr;
	}
}
void Bullet::Update(const float deltaTime,Entity& shooter){
	if(active&&!shouldRemove){
		if(isOnShoot){
			tryTriggerEffects(shooter, Occasion::OnShoot);
			EventSystem::Get().broadcastEvent(Occasion::OnHit, shooter);
			isOnShoot=false;
		}
		position.x+=velocity.x*deltaTime*BULLET::BASIC_SPEED;
		position.y+=velocity.y*deltaTime*BULLET::BASIC_SPEED;
		Draw();
		checkHit(shooter);	
	}
}
void Bullet::checkHit(Entity& shooter) {
	if(active&&Collision::checkBulletEntity(*this, shooter.getOpponent())){
		tryTriggerEffects(shooter, Occasion::OnHit);
		shouldRemove=true;
	}else{
		if(isOutOfScreen(this->getColliderCentre())){
			shouldRemove=true;
		}
	}
}
void Bullet::tryTriggerEffects(Entity& shooter,const Occasion& timing){
	for(auto& effect :effects){
		effect->tryTrigger(shooter, timing);
	}
}
void Bullet::Draw() const { 
	if(IsTextureValid(texture)){
		DrawTextureEx(texture,position,0,drawScale,WHITE); 
		DrawCircleLinesV(getColliderCentre(),colliderRadius,BLUE);
	}
}
void Bullet::DrawAsPattern(const Vector2& pos,float scale) const{
	if(IsTextureValid(texture)){
		if(scale==0.f){
			scale=drawScale;
		}
		DrawTextureEx(texture,pos,0,scale,WHITE);
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