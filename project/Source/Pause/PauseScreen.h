#pragma once
#include "../../Library/GameObject.h"
#include "../GameControler/GameControler.h"


class UIManager;

class PauseScreen : public GameObject
{
public:
	PauseScreen();
	~PauseScreen();

	void Update()override;
	void Draw()override;

	void PauseStart();
	bool PauseFinish();

	bool IsSelect() { return selectMenu; }

	enum PauseMenuItem
	{
		Back,
		Title
	};

	

private:
	bool pause;
	float timeRate;
	bool selectMenu;
	UIManager* UiManager;

	PauseMenuItem pauseItem;
};