#ifndef BULLET_H
#define BULLET_H
#include <vector>
extern "C"{
	#include "raylib.h"
}
#include <memory>
#include <string>
#include "GameStageFwd.h"

class Entity;
class BulletEffect;

namespace BULLET{
	//子弹
	const Vector2 BULLET_SIZE={240,240};
	const float BULLET_RADIUS=150.0f;
	//大招
	const float BLAST_DRAWSCALE=0.3f;
	//全局
	const int BASIC_SPEED=160 ;//基础速度，单位是每秒像素	
	const int BASIC_DMG=50;//基础伤害
}

//基础版本子弹，有子弹的基本功能，特效只有伤害
class Bullet{
private:
	Vector2 position;
	Texture2D texture;
	std::string texturePath;
	Vector2 velocity;//速度倍率
	float drawScale=0.1f;//子弹缩放倍率
	int damage;
	float colliderRadius;
	bool active;
	bool shouldRemove=false;
	std::vector<std::shared_ptr<BulletEffect>> effects;

public:
	~Bullet()=default;
	Bullet():shouldRemove(true),active(false){/*默认子弹不会出现在场上*/}
	Bullet(const std::string texPath,const Vector2& vel={0,1},const Vector2& pos={0,0},const int dmg=BULLET::BASIC_DMG,const bool act=false);
	Bullet(const Bullet& proto,const Vector2& begin);

	std::unique_ptr<Bullet> shoot(const Vector2& begin);
	void Draw() const;
	void Update(const float deltaTime,Entity& shooter);
	void checkHit(Entity& shooter);
	void tryTriggerEffects(Entity& shooter,const Occasion& timing) const;
	void addEffect(std::shared_ptr<BulletEffect> effect){effects.push_back(effect);}
	void setDamage(const int dmg){damage=dmg;}
	void setDrawScale(const float val){drawScale=val;}
	void countColliderRadius(const Vector2 size=BULLET::BULLET_SIZE);
	const Vector2 getColliderCentre() const {return {position.x+int(texture.width*drawScale/2),position.y+int(texture.height*drawScale/2)};}
	const float getColliderRadius() const {return colliderRadius;}
	const bool shouldBeRemoved() const{return shouldRemove;}
};
class Blast:public Bullet{

public:
	Blast():Bullet(){}
	Blast(const std::string texPath,const Vector2& vel={0,1},const Vector2& pos={0,0},const int dmg=10*BULLET::BASIC_DMG,const bool act=false)
	:Bullet(texPath,vel,pos,dmg,act){
		setDrawScale(BULLET::BLAST_DRAWSCALE);
		countColliderRadius(BULLET::BULLET_SIZE);
	}
	Blast(const Blast& proto,const Vector2& begin):Bullet(*this,begin){}
};


#endif