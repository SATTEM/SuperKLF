#ifndef TOOLS_H
#define TOOLS_H

#include "nlohmann/json.hpp"
#include <list>
class Entity;
class Bullet;
class Rectangle;
namespace Collision{
	bool checkBulletEntity(const Bullet& aBullet,const Entity& aEntity);
	bool checkEntityEntity(const Entity& a,const Entity& b);
	bool checkMouseTouch(const Rectangle& rect);
}
namespace Check{
	const bool isJsonValid(const nlohmann::json& j,const std::list<std::string>& items);
}
namespace Trans{
	std::string wstrToUTF(const std::wstring&);
	std::wstring UTFTowstr(const std::string&);
}
#endif
