#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H
#include "Effect/RelicEffect.h"
#include <memory>
#include <unordered_map>
#include <vector>
class Entity;
enum class Occasion{
	OnShoot,
	OnHit,
	Instant,
	Always,
	OnBattleStart,
	OnPlayerHit,
	OnHPThreshold,
	Undecided
};
class EventSystem{
private:
	EventSystem()=default;
	~EventSystem()=default;
	std::unordered_map<Occasion,std::vector<std::shared_ptr<RelicEffect>>> listeners;
public:
	//将某个遗物和事件绑定在一起
	void bindRelicAndEvent(std::shared_ptr<RelicEffect>,Occasion);
	//广播某事件
	void broadcastEvent(Occasion);

};
#endif