#ifndef ENTITY_H
#define ENTITY_H
extern "C"{
	#include "raylib.h"
}
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include "Bullet.h"

class Entity{
protected:
	Vector2 position;
	Rectangle boxCollider;
	float attackInterval;//unit:second
	float attackTimer;
	int maxHP;
	int currentHP;
	int energy,maxEnergy,energyRise;
	Texture2D texture;
	std::vector<Bullet> bulletPattern;
	std::vector<std::unique_ptr<Bullet>> bulletPool;
	std::size_t bulletIndex;
public:
	explicit Entity(const std::string texPath,const Vector2& pos,const int hp=100,
	const float interval=1,const int MAXenergy=100,const int rise=10,
	const std::vector<Bullet>& pattern={});

	~Entity()=default;
	virtual void Update(Entity& opponent,const float deltaTime)=0;
	virtual void Draw() const =0;
	void fireBullet(Vector2 pos={0,0});
	void takeDamage(const int damage){
		currentHP=std::max(0,currentHP-damage);
	}
	bool isAlive() const {return currentHP>=0;}
	bool canCastSkill() const{return energy>=maxEnergy;}
	void addEnergy(const int value){
		energy=std::clamp(energy+value,0,maxEnergy);
	}
	virtual void drawHPandEnergy() const=0;
	void resetEnergy(){energy=0;}
	void addBullet(Bullet aBullet){bulletPattern.push_back(aBullet);}
	void removeBullet(const int index){bulletPattern.erase(bulletPattern.begin()+index);}
	float& getAttackTimer(){return attackTimer;}
	float& getAttackInterVale(){return attackInterval;}
	int& getEnergyRise(){return energyRise;}
	const int getEnergy() const{return energy;}
	const int getHP() const{return currentHP;}
	const int getMaxEnergy() const{return maxEnergy;}
	const Rectangle& getCollider() const{return boxCollider;}

};

class Player:public Entity{
public:
	Player(const std::string texPath,const Vector2& pos,const int hp=100,const float interval=1,const int MAXenergy=100,const int rise=10)
	:Entity(texPath,pos,hp,interval,MAXenergy,rise){}
	void Update(Entity& opponent,const float deltaTime) override;
	void Draw() const override;
	void drawHPandEnergy() const override;
};

class Enemy:public Entity{
public:
	Enemy(const std::string texPath,const Vector2& pos,const int hp=100,const float interval=1,const int MAXenergy=100,const int rise=10)
	:Entity(texPath,pos,hp,interval,MAXenergy,rise){}
	void Update(Entity& opponent,const float deltaTime) override;
	void Draw() const override;
	void drawHPandEnergy() const override;
};
#endif