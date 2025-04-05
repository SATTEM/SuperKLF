#ifndef ENTITY_H
#define ENTITY_H
extern "C"{
	#include "raylib.h"
}
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include "Bullet.h"
#include "Effect/RelicEffect.h"
#include "UI/Information.h"
namespace GameInit{
	const int Start_Money=33;
}
class Entity{
protected:
	Vector2 position;
	Rectangle boxCollider;
	Texture2D texture;
	float attackInterval,attackTimer;//unit:second
	int maxHP,currentHP,energy,maxEnergy,energyRise;
	Entity* opponent;
	Blast blast;
	std::vector<Bullet> bulletPattern;
	std::size_t bulletIndex;
	std::vector<std::unique_ptr<Bullet>> bulletPool;
	std::vector<std::shared_ptr<RelicEffect>> relics;
public:
	explicit Entity(const std::string texPath,const Vector2& pos,const int hp=100
	,const float interval=1,const int MAXenergy=100,const int rise=10
	,const std::vector<Bullet>& pattern={});

	~Entity()=default;
	virtual void Update(const float deltaTime)=0;
	void fire(Vector2 pos={0,0});
	void fireBlast();
	void reset();
	
	void takeDamage(const int damage);
	bool isAlive() const {return currentHP>0;}
	bool canCastSkill() const{return energy>=maxEnergy;}
	
	virtual void addRelic(std::shared_ptr<RelicEffect> relic);
	virtual void removeRelic(const int);
	void addBullet(Bullet aBullet){bulletPattern.push_back(std::move(aBullet));}
	void removeBullet(const int index){bulletPattern.erase(bulletPattern.begin()+index);}
	void setBlast(Blast aBlast){blast=std::move(aBlast);}
	void setOpponent(Entity& opp){opponent=&opp;}

	float& getAttackTimer(){return attackTimer;}
	float& getAttackInterVale(){return attackInterval;}
	Entity& getOpponent(){return *opponent;}
	const int getEnergy() const{return energy;}
	const int getHP() const{return currentHP;}
	const int getMaxEnergy() const{return maxEnergy;}
	const std::vector<Bullet>& getBulletPattern() const{return bulletPattern;}
	const Rectangle& getCollider() const{return boxCollider;}
private:
	void addEnergy(const int value){energy=std::clamp(energy+value,0,maxEnergy);}
	virtual void Draw() const =0;
	virtual void drawHPandEnergy() const=0;
	void resetEnergy(){energy=0;}
};

class Player:public Entity{
private:
	int money=GameInit::Start_Money;
	std::vector<TextureDetailedDisplay> relicDisplays;
public:
	Player(const std::string texPath,const Vector2& pos,const int hp,const float interval,const int MAXenergy,const int rise);
	void Update(const float deltaTime) override;
	void setAttackInterval(const float rate){attackInterval*=rate;}
	void addRelic(std::shared_ptr<RelicEffect> relic) override;
	void removeRelic(const int) override;
	void addMoney(const int value){money+=value;}
	const bool deductMoney(const int value){
		if(money-value<0){return false;}
		else{money-=value;return true;}
	}
	void MaxHealthBoost(const int val){maxHP+=val;}
	void MaxHealthBoost(const float rate){maxHP*=rate;}

	const int getMoney() const{return money;}
private:
	void Draw() const override;
	void drawHPandEnergy() const override;
};

class Enemy:public Entity{
public:
	Enemy(const std::string texPath,const Vector2& pos,const int hp=100
	,const float interval=1,const int MAXenergy=100,const int rise=10)
	:Entity(texPath,pos,hp,interval,MAXenergy,rise){}
	void Update(const float deltaTime) override;
private:
	void Draw() const override;
	void drawHPandEnergy() const override;
};
#endif