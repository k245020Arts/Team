#pragma once
#include "../../TrashEnemy/EnemyState/EnemyStateBase.h"

class BossWalk :public EnemyStateBase
{
public:
	BossWalk();
	~BossWalk();

	void Update()override;

	void Start()override;
	void Finish()override;
private:
	float timeCounter;

	int maxCounter;
};