#include "Entity.h"
#include "Bullet.h"
#include "Event/EventFWD.h"
#include "ResourceManager.h"
#include <algorithm>
#include <memory>
#include "Event/EventSystem.h"
extern "C"{
	#include "raylib.h"
}
#include "UI/UIUtility.h"
void updateBulletPool(std::vector<std::unique_ptr<Bullet>>& pool,const float deltaTime,Entity& shooter);
void tryFire(Entity& aEntity,const float deltaTime);
void updateCollision(std::vector<std::unique_ptr<Bullet>>& pool,const float deltaTime,Entity& shooter);
bool triggerHPThreshold(const int hp,const int maxHP);



Entity::Entity(const std::string texPath,const Vector2& pos,const int hp,const float interval,const int MAXenergy,const int rise,const std::vector<Bullet>& pattern)
		:position(pos),maxHP(hp),attackInterval(interval),
		 attackTimer(0),energy(0),maxEnergy(MAXenergy),energyRise(rise),
		 bulletIndex(0),bulletPattern(pattern),currentHP(hp),
		 bulletPool{}{
			texture=ResourceManager::Get().loadTexture(texPath);
			if(!IsTextureValid(texture)){
				//当加载不成功时，使用临时图标
				TraceLog(LOG_WARNING, "Using fallback texture for: %s",texPath.c_str());
				texture=ResourceManager::Get().loadTexture(ASSETS_IMAGE_PATH "/fallback.png");
			}
			boxCollider={position.x,position.y,(float)texture.width,(float)texture.height};
		 }
void Entity::reset(){
	currentHP=maxHP;
	energy=0;
	bulletIndex=0;
	bulletPool.clear();
}
void Entity::fire(Vector2 pos){
	if(bulletPattern.empty()){
		return;
	}
	if(pos.x==0&&pos.y==0){
		pos.x=position.x;
		pos.y=position.y;
	}
	bulletPool.push_back(bulletPattern[bulletIndex].shoot(pos));
	bulletIndex=(bulletIndex+1) % bulletPattern.size();
	addEnergy(energyRise);
	EventSystem::Get().broadcastEvent(Occasion::OnShoot,*this);
}
void Entity::fireBlast(){
	bulletPool.push_back(blast.shoot(position));
	resetEnergy();
	EventSystem::Get().broadcastEvent(Occasion::OnBlastShoot,*this);
}
void Entity::addRelic(std::shared_ptr<RelicEffect> relic){
	relics.push_back(std::move(relic));
	EventSystem::Get().bindRelicAndEvent(relics.back());
}
void Entity::removeRelic(const int i){
	EventSystem::Get().unbindRelicAndEvent(relics[i],relics[i]->getOccasion());
	relics.erase(relics.begin()+i);
}
void Entity::takeDamage(const int damage){
	currentHP=std::max(0,currentHP-damage);
	if(triggerHPThreshold(currentHP, maxHP)){
		EventSystem::Get().broadcastEvent(Occasion::OnHPThreshold, *this);
	}
}
bool triggerHPThreshold(const int hp,const int maxHP){
	if(hp==maxHP*0.75){
		return true;
	}else if(hp==maxHP/2){
		return true;
	}else if(hp==maxHP/3){
		return true;
	}else if(hp==maxHP/4){
		return true;
	}
	return false;
}
/*--------------------------------------------------*/
Player::Player(const std::string texPath,const Vector2& pos,const int hp=100
	,const float interval=1,const int MAXenergy=100,const int rise=10)
:Entity(texPath,pos,hp,interval,MAXenergy,rise){}

void Player::Update(const float deltaTime){
	tryFire(*this, deltaTime);
	updateBulletPool(bulletPool, deltaTime,*this);
	Draw();
}

void Player::addRelic(std::shared_ptr<RelicEffect> relic){
	Entity::addRelic(relic);
	auto pos=UI::countRelicDisplayPos(relicDisplays.size());
	TextureDetailedDisplay tdd(
		relic->getIcon(),
		relic->getDescription(),
		UI::countRelicDisplayPos(relicDisplays.size()),
		UI::RELIC_DISPLAY_MAX_LENGTH);
	relicDisplays.push_back(tdd);
}
void Player::removeRelic(const int i){
	Entity::removeRelic(i);
	relicDisplays.erase(relicDisplays.begin()+i);
	for(int j=0;j<relicDisplays.size()-i;j++){
		relicDisplays[i+j].setPosition(UI::countRelicDisplayPos(i+j));
	}
}
void Player::Draw() const {
	DrawTextureV(texture,position,WHITE);
	drawHPandEnergy();
	DrawRectangleLinesEx(boxCollider,5,RED);
	for(const auto& rd:relicDisplays){
		rd.Draw();
	}
}
void Player::drawHPandEnergy() const{
	DrawRectangleLines(0, GetScreenHeight()-40, int(GetScreenWidth()/2), 40, BLACK);
	DrawRectangle(0, GetScreenHeight()-40, int((float(currentHP)/maxHP)*(float(GetScreenWidth())/2)), 40, RED);
	DrawRectangleLines(int(GetScreenWidth()/2), GetScreenHeight()-40, int(GetScreenWidth()/2), 40, BLACK);
	DrawRectangle(int(GetScreenWidth()/2), GetScreenHeight()-40, int((float(energy)/maxEnergy)*(float(GetScreenWidth())/2)), 40, ORANGE);	
}
/*--------------------------------------------------*/
void Enemy::Update(const float deltaTime){
	tryFire(*this, deltaTime);
	updateBulletPool(bulletPool, deltaTime,*this);
	Draw();
}
void tryFire(Entity& shooter,const float deltaTime){
	float& attackTimer=shooter.getAttackTimer();
	float& attackInterval=shooter.getAttackInterVale();
	attackTimer+=deltaTime;
	if(shooter.getEnergy()==shooter.getMaxEnergy()){
		shooter.fireBlast();
	}	
	if(attackTimer>=attackInterval){
		shooter.fire();
		attackTimer=0;
	}
	EventSystem::Get().broadcastEvent(Occasion::OnShoot,shooter);
}

void updateBulletPool(std::vector<std::unique_ptr<Bullet>>& pool,const float deltaTime,Entity& shooter){
	for(auto it=pool.begin();it!=pool.end();){
		(*it)->Update(deltaTime,shooter);
		if((*it)->shouldBeRemoved()){
			it=pool.erase(it);
		}else{
			++it;
		}
	}	
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

