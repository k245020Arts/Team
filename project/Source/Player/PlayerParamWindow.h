#pragma once
#include "Player.h"

class PlayerParamWindow
{
public:
	PlayerParamWindow();
	PlayerParamWindow(Player* _player);
	~PlayerParamWindow();

	void PlayerParamWindowView();
	void SaveReaction();
	void SaveAttackData();

	void LoadAttackData();
	void LoadReaction();


private:
	bool windowMode;
	Player* player;
};