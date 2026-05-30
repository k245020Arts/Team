#pragma once
#include "CameraStateBase.h"

class TrashEnemyManager;
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
};
