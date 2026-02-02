#pragma once
#include "../Component.h"

class UIManager;

class UIBase : public Component
{
public:
	UIBase();
	~UIBase();

	void Awake()override;

private:
	UIManager* uiManager;
};