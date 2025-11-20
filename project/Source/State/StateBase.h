#pragma once
#include "../../Library/GameObject.h"
#include "../Player/PlayerInformation.h"
#include <typeinfo>
#include "../Common/ID/ID.h"
#include "../Component/Component.h"

class BaseObject;

class StateBase
{
public:
	StateBase() {
		id = ID::ID_MAX;
		string = "";
		obj = nullptr;
		com = nullptr;
	}
	~StateBase(){}

	void Init(BaseObject* _base) {
		obj = _base;
	}

	virtual void Update(){}
	virtual void Draw(){}
	virtual void Start() { ; }

	std::string GetString() { return string; }
	ID::IDType GetID() { return id; }

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
	ID::IDType id;
	BaseObject* obj;
	Component* com;
};