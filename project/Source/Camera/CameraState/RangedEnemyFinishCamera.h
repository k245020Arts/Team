#pragma once
#include "CameraStateBase.h"

class TrashEnemyGroup;
class Player;

class RangedEnemyFinishCamera :public CameraStateBase
{
public:
	RangedEnemyFinishCamera();
	~RangedEnemyFinishCamera();

	void Update()override;
	void Start()override;
	void Finish()override;

private:
	Player* player;
	TrashEnemyGroup* groupManager;
	VECTOR3 targetPos;
	VECTOR3 lookPos;

	const VECTOR3 PosOffset = VECTOR3(800.0f, 600.0f, -800.0f);

	VECTOR3 keepTarget;
	float timer;
	VECTOR3 keepPos;

	const float MAX_TIMER = 0.8f;
};
