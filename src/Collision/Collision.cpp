#include "Collision.h"
#include <algorithm>
extern "C"{
	#include <raylib.h>
}
using std::clamp;

bool Collision::checkBulletEntity(const Bullet &aBullet, const Entity &aEntity){
	Vector2 bulletCentre=aBullet.getColliderCentre();
	float bulletRadiu=aBullet.getColliderRadius();

	Rectangle entityBox=aEntity.getCollider();

	//计算矩形边界点与圆心最近的距离
	float closestX=clamp(bulletCentre.x,entityBox.x,entityBox.x+entityBox.width);
	float closestY=clamp(bulletCentre.y,entityBox.y,entityBox.y+entityBox.height);
	float distanceX=bulletCentre.x-closestX;
	float distanceY=bulletCentre.y-closestY;
	float distanceSquare=(distanceX*distanceX)+(distanceY*distanceY);
	return distanceSquare<=(bulletRadiu*bulletRadiu);
}
bool Collision::checkEntityEntity(const Entity &a, const Entity &b){
	return CheckCollisionRecs(a.getCollider(), b.getCollider());
}