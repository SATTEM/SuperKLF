#ifndef COLLISION_H
#define COLLISION_H

#include "Bullet.h"
#include "Entity.h"
#include "UI/Button.h"
namespace Collision{
	bool checkBulletEntity(const Bullet& aBullet,const Entity& aEntity);
	bool checkEntityEntity(const Entity& a,const Entity& b);
	bool checkIsTouchButton(const Button& aButton);
}

#endif
