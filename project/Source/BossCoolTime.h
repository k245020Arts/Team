#pragma once
#include "enemyStateBase.h"

class BossCoolTime:public EnemyStateBase
{
public:
	BossCoolTime();
	~BossCoolTime();

	void Update()override;

	void Start()override;
	void Finish()override;

private:
	const float COOLTIME = 60.0f;
	float coolTime;
};