#pragma once
#include <string>

namespace StateID{

	enum State_ID
	{

	};

	std::string GetID(State_ID _type);
	void SetID(std::string _state, State_ID _type);
}