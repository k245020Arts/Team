#pragma once
#include "CameraStateBase.h"

class TrashEnemyManager;
class Player;

class RangedEnemyCamera :public CameraStateBase
{
public:
	RangedEnemyCamera();
	~RangedEnemyCamera();

	void Update()override;
	void Draw()override;
	void Start()override;
	void Finish()override;

private:
	TrashEnemyManager* tEnemyManager;
	Player* player;

	const VECTOR3 camPos = VECTOR3(0, 50, -700);
	VECTOR3 enemyPos;
	VECTOR3 pPos;
};