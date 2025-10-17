#pragma once
#include "enemyStateBase.h"

static const float BOSS_SPEED = 5.0f;

class BossRun : public EnemyStateBase
{
public:
	BossRun();
	~BossRun();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	VECTOR3 velocity;
	VECTOR3 rotation;
};