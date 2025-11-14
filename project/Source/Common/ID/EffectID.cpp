#include "EffectID.h"
#include <unordered_map>
#include <assert.h>
#include "../Debug/Debug.h"

namespace {
	std::unordered_map<int, std::string> effectId;
}

std::string Effect_ID::GetEffectID(EFFECT_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)EFFECT_MAX) {
		Debug::CreateMessageBox("EffectHandleMiss", "miss");
	}
	return effectId[type];
}

void Effect_ID::SetEffectID(std::string _effect, EFFECT_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)EFFECT_MAX) {
		Debug::CreateMessageBox("EffectHandleMiss", "miss");
	}
	effectId[type] = _effect;
}
