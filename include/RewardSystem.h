#ifndef REWARDSYSTEM_H
#define REWARDSYSTEM_H

#include <memory>
#include <string>
#include "Effect/InstantEffect.h"
#include "Entity.h"
extern "C"{
	#include "raylib.h"
}
class Entity;
class Reward{
protected:
	std::wstring name;
	std::wstring description;
	std::shared_ptr<InstantEffect> effect;
	Texture2D icon;
public:
	Reward(const nlohmann::json& reward);
	~Reward()=default;
	void apply(Player& player);
	const std::wstring& getDescription()const{return description;}
	const std::wstring& getName()const{return name;}
};
#endif