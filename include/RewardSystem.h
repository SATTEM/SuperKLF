#ifndef REWARDSYSTEM_H
#define REWARDSYSTEM_H

#include <memory>
#include <string>
#include "Effect/InstantEffect.h"
extern "C"{
	#include "raylib.h"
}
class Entity;
struct Reward{
	std::string name;
	std::string description;
	std::shared_ptr<InstantEffect> effect;
	Texture2D icon;
	void applyToEntity(Entity& entity);
	void getEffect(const std::string& id);
};

#endif