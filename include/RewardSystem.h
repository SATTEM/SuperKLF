#ifndef REWARDSYSTEM_H
#define REWARDSYSTEM_H

#include <string>
#include <memory>
#include <vector>
#include "nlohmann/json.hpp"
#include "Effect/InstantEffect.h"
extern "C"{
	#include "raylib.h"
}

struct Reward{
	std::string name;
	std::string description;
	std::vector<std::shared_ptr<InstantEffect>> effects;
	Texture2D icon;
	void getEffects(const nlohmann::json& j);
};

#endif