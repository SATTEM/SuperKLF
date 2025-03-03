#ifndef INSTANTEFFECTFACTORY_H
#define INSTANTEFFECTFACTORY_H
#include "InstantEffect.h"
#include "nlohmann/json.hpp"
#include <memory>
class InstantEffectFactory{
public:
	//创建一个即时效果，需要的固定参数都由加载时内置
	//可变参数由set()相关方法设置（需要时特化）
	virtual std::shared_ptr<InstantEffect> create()=0;
	virtual void loadFromJson(const nlohmann::json&)=0;
	InstantEffectFactory()=default;
	virtual ~InstantEffectFactory()=default;
};

template<typename T>
class ConcreteInstantEffectFactory:public InstantEffectFactory{
private:
	nlohmann::json params;
public:
	std::shared_ptr<InstantEffect> create() override{
		return std::make_shared<T>(params);
	}
	void loadFromJson(const nlohmann::json& p) override{
		params=p;
	}
};


#endif