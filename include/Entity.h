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
	virtual void Draw() const =0;
	virtual void drawHPandEnergy() const=0;
	void fire(Vector2 pos={0,0});
	void fireBlast();
	void takeDamage(const int damage);
	bool isAlive() const {return currentHP>0;}
	bool canCastSkill() const{return energy>=maxEnergy;}
	void addEnergy(const int value){
		energy=std::clamp(energy+value,0,maxEnergy);
	}
	void resetEnergy(){energy=0;}
	void reset();
	void addBullet(Bullet aBullet){bulletPattern.push_back(std::move(aBullet));}
	void removeBullet(const int index){bulletPattern.erase(bulletPattern.begin()+index);}
	void setBlast(Blast aBlast){blast=std::move(aBlast);}
	void setOpponent(Entity& opp){opponent=&opp;}
	void addRelic(std::shared_ptr<RelicEffect> relic);
	float& getAttackTimer(){return attackTimer;}
	float& getAttackInterVale(){return attackInterval;}
	int& getEnergyRise(){return energyRise;}
	Entity& getOpponent(){return *opponent;}
	const int getEnergy() const{return energy;}
	const int getHP() const{return currentHP;}
	const int getMaxEnergy() const{return maxEnergy;}
	const Rectangle& getCollider() const{return boxCollider;}

};

class Player:public Entity{
private:
	int money=GameInit::Start_Money;
public:
	Player(const std::string texPath,const Vector2& pos,const int hp=100
	,const float interval=1,const int MAXenergy=100,const int rise=10)
	:Entity(texPath,pos,hp,interval,MAXenergy,rise){}
	void Update(const float deltaTime) override;
	void Draw() const override;
	void drawHPandEnergy() const override;
	void setAttackInterval(const float rate){attackInterval*=rate;}
	void MaxHealthBoost(const int val){maxHP+=val;}
	void MaxHealthBoost(const float rate){maxHP*=rate;}
	void addMoney(const int value){money+=value;}
	const bool deductMoney(const int value){
		if(money-value<0){return false;}
		else{money-=value;return true;}
	}
	const int getMoney() const{return money;}
};

class Enemy:public Entity{
public:
	Enemy(const std::string texPath,const Vector2& pos,const int hp=100
	,const float interval=1,const int MAXenergy=100,const int rise=10)
	:Entity(texPath,pos,hp,interval,MAXenergy,rise){}
	void Update(const float deltaTime) override;
	void Draw() const override;
	void drawHPandEnergy() const override;
};
#endif