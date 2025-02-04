#ifndef BULLET_H
#define BULLET_H
extern "C"{
	#include "raylib.h"
}
#include <memory>
#include <string>


class Entity;
namespace BULLET{
	//子弹的大小
	const int BULLET_HEIGHT=24;
	const int BULLET_WIDTH=24;
	const float BULLET_RADIUS=15.0f;//子弹碰撞半径
	const int BASIC_SPEED=160 ;//基础速度，单位是每秒像素	
}

class Bullet{
private:
	Vector2 position;
	Texture2D texture;
	Vector2 velocity;//速度倍率
	int damage;
	float colliderRadius;
	bool active;
	bool shouldRemove=false;

public:
	Bullet(const std::string texPath,const Vector2& vel={0,1},const Vector2& pos={0,0},const int dmg=1,const bool act=false);
	Bullet(const Texture2D& tex,const Vector2& vel={0,1},const Vector2& pos={0,0},const int dmg=1,const bool act=true)
		:position(pos),velocity(vel),active(act),colliderRadius(BULLET::BULLET_RADIUS),damage(dmg),texture(tex){}

	std::unique_ptr<Bullet> shoot(const Vector2& begin);
	~Bullet()=default;
	void Draw() const;
	void Update(const float deltaTime,Entity& aEntity);
	void checkHit(Entity& aEntity);
	void setDamage(const int dmg){damage=dmg;}
	const Vector2 getColliderCentre() const {
		return {position.x+int(BULLET::BULLET_WIDTH/2),position.y+int(BULLET::BULLET_HEIGHT/2)};
		}
	const float getColliderRadius() const {return colliderRadius;}
	const bool shouldBeRemoved() const{return shouldRemove;}
	
	
	
};


#endif