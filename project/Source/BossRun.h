#pragma once
#include "enemyStateBase.h"

class AttackSorting;
class BossStatus;

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
	BossStatus* bs;
	VECTOR3 velocity;
	VECTOR3 rotation;
	float speed;
};