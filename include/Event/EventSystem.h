#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H
#include "Effect/RelicEffect.h"
#include <memory>
#include <unordered_map>
#include <list>
#include <algorithm>
#include "EventFWD.h"
class Entity;

class EventSystem{
private:
	EventSystem()=default;
	~EventSystem()=default;
	std::unordered_map<Occasion,std::list<std::shared_ptr<RelicEffect>>> listeners;
public:
	static EventSystem& Get(){
		static EventSystem instance;
		return instance;
	}
	//将遗物，事件绑定在一起
	void bindRelicAndEvent(std::shared_ptr<RelicEffect>& relic){
		Occasion event=relic->getOccasion();
		listeners[event].push_back(relic);
	}
	//广播某事件，同时传入相关的实体
	void broadcastEvent(Occasion event,Entity& relatedEntity){
		if(listeners.find(event)==listeners.end()){
			listeners[event]={};
		}
		for(auto& relic:listeners[event]){
			relic->onTrigger(relatedEntity);
		}
	}
	//解除绑定
	void unbindRelicAndEvent(std::shared_ptr<RelicEffect> relic,Occasion event){
		auto it=std::find_if(listeners[event].begin(),listeners[event].end(),
		[&relic](std::shared_ptr<RelicEffect>& iterator)->bool{return iterator==relic;});
		listeners[event].erase(it);
	}

};
#endif