#pragma once
#include "../../Library/GameObject.h"
#include "../Player/PlayerInformation.h"
#include <typeinfo>
#include "../Component/Component.h"
#include "../Common/ID/StateID.h"

class BaseObject;

class StateBase
{
public:
	StateBase() {
		id = StateID::STATE_MAX;
		string = "";
		obj = nullptr;
		com = nullptr;
	}
	~StateBase(){}

	void Init(BaseObject* _base,StateID::State_ID _id) {
		obj = _base;
		id = _id;
	}

	virtual void Update(){}
	virtual void Draw(){}
	virtual void Start() { ; }

	std::string GetString() { return string; }
	StateID::State_ID GetID() { return id; }

	virtual void AvoidStart() { ; }
	virtual void Finish() { ; }

	template<typename T>
	void SetComponent(T* _c) {
		com = _c;
	}

	template<typename T> 
	T* GetBase() {
		return dynamic_cast<T*>(com);
	}

protected:
	std::string string;
	StateID::State_ID id;
	BaseObject* obj;
	Component* com;
};