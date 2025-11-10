#pragma once
#include "../Library/GameObject.h"
#include "Transitor.h"

class TransitorManager : public GameObject
{
public:
	TransitorManager();
	~TransitorManager();

	bool IsTransitor();

private:
	std::list<Transitor*> list;
};