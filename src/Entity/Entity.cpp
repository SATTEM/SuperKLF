#include "Entity.h"
#include "Bullet.h"
#include "ResourceManager.h"
#include <memory>

extern "C"{
	#include "raylib.h"
}

void updateBulletPool(std::vector<std::unique_ptr<Bullet>>& pool,const float deltaTime,Entity& aEntity);
void tryFire(Entity& aEntity,const float deltaTime);
void updateCollision(std::vector<std::unique_ptr<Bullet>>& pool,const float deltaTime,Entity& aEntity);

Entity::Entity(const std::string texPath,const Vector2& pos,const int hp,const float interval,const int MAXenergy,const int rise,const std::vector<Bullet>& pattern)
		:position(pos),maxHP(hp),attackInterval(interval),
		 attackTimer(0),energy(0),maxEnergy(MAXenergy),energyRise(rise),
		 bulletIndex(0),bulletPattern(pattern),currentHP(hp),
		 bulletPool{}{
			texture=ResourceManager::Get().loadTexture(texPath);
			if(!IsTextureValid(texture)){
				//当加载不成功时，使用临时图标
				TraceLog(LOG_WARNING, "Using fallback texture for: %s",texPath.c_str());
				texture=ResourceManager::Get().loadTexture(ASSETS_PATH "/fallback.png");
			}
			boxCollider={position.x,position.y,(float)texture.width,(float)texture.height};
		 }

void Entity::fireBullet(Vector2 pos){
	if(bulletPattern.empty()){
		return;
	}
	if(pos.x==0&&pos.y==0){
		pos.x=position.x;
		pos.y=position.y;
	}
	bulletPool.push_back(bulletPattern[bulletIndex].shoot(pos));
	addEnergy(energyRise);
	bulletIndex=(bulletIndex+1) % bulletPattern.size();
}
void Player::Update(Entity& opponent,const float deltaTime){
	tryFire(*this, deltaTime);
	updateBulletPool(bulletPool, deltaTime,opponent);
}
void Player::Draw() const {
	DrawTextureV(texture,position,WHITE);
	drawHPandEnergy();
	DrawRectangleLinesEx(boxCollider,5,RED);
}
void Player::drawHPandEnergy() const{
	DrawRectangleLines(0, GetScreenHeight()-40, int(GetScreenWidth()/2), 40, BLACK);
	DrawRectangle(0, GetScreenHeight()-40, int((float(currentHP)/maxHP)*(float(GetScreenWidth())/2)), 40, RED);
	DrawRectangleLines(int(GetScreenWidth()/2), GetScreenHeight()-40, int(GetScreenWidth()/2), 40, BLACK);
	DrawRectangle(int(GetScreenWidth()/2), GetScreenHeight()-40, int((float(energy)/maxEnergy)*(float(GetScreenWidth())/2)), 40, ORANGE);	
}
void Enemy::Update(Entity& opponent,const float deltaTime){
	tryFire(*this, deltaTime);
	updateBulletPool(bulletPool, deltaTime,opponent);
}
void Enemy::Draw() const {
	DrawTextureV(texture,position,WHITE);
	drawHPandEnergy();
	DrawRectangleLinesEx(boxCollider,5,RED);
}
void Enemy::drawHPandEnergy() const{
	DrawRectangleLines(0, 0, int(GetScreenWidth()/2), 40, BLACK);
	DrawRectangle(0, 0, int((float(currentHP)/maxHP)*(float(GetScreenWidth())/2)), 40, RED);
	DrawRectangleLines(int(GetScreenWidth()/2), 0, int(GetScreenWidth()/2), 40, BLACK);
	DrawRectangle(int(GetScreenWidth()/2), 0, int((float(energy)/maxEnergy)*(float(GetScreenWidth())/2)), 40, ORANGE);	
}

void tryFire(Entity& aEntity,const float deltaTime){
	float& attackTimer=aEntity.getAttackTimer();
	float& attackInterval=aEntity.getAttackInterVale();
	attackTimer+=deltaTime;
	if(attackTimer>=attackInterval){
		aEntity.fireBullet();
		attackTimer=0;
		aEntity.addEnergy(aEntity.getEnergyRise());
	}	
}

void updateBulletPool(std::vector<std::unique_ptr<Bullet>>& pool,const float deltaTime,Entity& aEntity){
	updateCollision(pool, deltaTime,aEntity);
}

void updateCollision(std::vector<std::unique_ptr<Bullet>>& pool,const float deltaTime,Entity& aEntity){
	for(auto it=pool.begin();it!=pool.end();){
		(*it)->Update(deltaTime,aEntity);
		if((*it)->shouldBeRemoved()){
			it=pool.erase(it);
		}else{
			++it;
		}
	}	
}