#pragma once
#include "Player.h"

class PlayerParamWindow
{
public:
	PlayerParamWindow();
	PlayerParamWindow(Player* _player);
	~PlayerParamWindow();

	void PlayerParamWindowView();
	


private:
	bool windowMode;
	Player* player;

	void SaveReaction();
	void SaveAttackData();

	void LoadAttackData();
	void LoadReaction();

};