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
	//プレイヤーとカメラをどれくらい離すか
	const VECTOR3 PosOffset = VECTOR3(800.0f, 1000.0f, -800.0f);
	//敵の見る位置を少し上にする
	const VECTOR3 EnemyPosOffset = VECTOR3(0, 500, 0);

	VECTOR3 keepTarget;
	float timer;
	VECTOR3 keepPos;

	const float MaxTimer = 0.8f;
};
