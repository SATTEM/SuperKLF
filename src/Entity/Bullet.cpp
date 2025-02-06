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
#include "SpecialEffect.h"

const bool isOutOfScreen(const Vector2& pos);

Bullet::Bullet(const std::string texPath,const Vector2& vel,const Vector2& pos,const int dmg,const bool act)
	:position(pos),velocity(vel),active(act),damage(dmg),texturePath(texPath){
		Texture2D origin=ResourceManager::Get().loadTexture(texPath);
		texturePath=ResourceManager::Get().resizeTexture(texPath, BULLET::BULLET_SIZE.x, BULLET::BULLET_SIZE.y);
		texture=ResourceManager::Get().loadTexture(texturePath);
		countColliderRadius();
		effects.push_back(std::shared_ptr<BulletEffect>{});
		effects.back().reset(new GiveDamage(damage));
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
		return std::unique_ptr<Bullet>{new Bullet(*this,begin)};
	}else{
		return nullptr;
	}
}
void Bullet::Update(const float deltaTime,Entity& shooter){
	if(active&&!shouldRemove){
		//只有克隆体是“实例”
		position.x+=velocity.x*deltaTime*BULLET::BASIC_SPEED;
		position.y+=velocity.y*deltaTime*BULLET::BASIC_SPEED;
		Draw();
		checkHit(shooter);	
	}
}
void Bullet::checkHit(Entity& shooter) {
	//判断是否碰到目标，如果碰到了，触发效果（包括伤害），并返回true，否则返回false
	if(Collision::checkBulletEntity(*this, shooter.getOpponent())){
		tryTriggerEffects(shooter, Occasion::OnHit);
		shouldRemove=true;
	}else{
		if(isOutOfScreen(this->getColliderCentre())){
			shouldRemove=true;
		}
	}
}
void Bullet::tryTriggerEffects(Entity& shooter,const Occasion& timing) const{
	for(auto effect:effects){
		effect->tryTrigger(shooter,timing);
	}
}
void Bullet::Draw() const { 
	if(IsTextureValid(texture)){
		DrawTextureEx(texture,position,0,drawScale,WHITE); 
		DrawCircleLinesV(getColliderCentre(),colliderRadius,BLUE);
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