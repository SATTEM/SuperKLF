#ifndef TOOLS_H
#define TOOLS_H

#include "Bullet.h"
#include "Entity.h"
#include "UI/Button.h"
#include "nlohmann/json.hpp"
#include <list>
namespace Collision{
	bool checkBulletEntity(const Bullet& aBullet,const Entity& aEntity);
	bool checkEntityEntity(const Entity& a,const Entity& b);
	bool checkMouseTouch(const Rectangle& rect);
}
namespace Check{
	const bool isJsonValid(const nlohmann::json& j,const std::list<std::string>& items);
}
#endif
