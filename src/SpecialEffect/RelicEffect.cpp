#include "Effect/RelicEffect.h"
#include "Entity.h"

DoubleShootRelic::DoubleShootRelic(const nlohmann::json& params):RelicEffect(){
	description=params["description"].get<std::string>();
}
void DoubleShootRelic::onTrigger(Entity& relatedEntity){
	TraceLog(LOG_WARNING,"Triggering DoubleShootRelic(Not developed yet)");
}