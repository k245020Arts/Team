#include "StateID.h"
#include "ID.h"
#include <unordered_map>
#include <assert.h>
#include "../Debug/Debug.h"

namespace {
	std::unordered_map<int, std::string> stateId;
}

void StateID::DeleteStateID()
{
	stateId.clear();
}

std::string StateID::GetID(State_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)STATE_MAX) {
		Debug::CreateMessageBox("StateHandleMiss", "miss");
	}
	return stateId[type];
}

void StateID::SetID(std::string _state, State_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)STATE_MAX) {
		Debug::CreateMessageBox("StateSetMiss", "miss");
	}
	stateId[type] = _state;
}
