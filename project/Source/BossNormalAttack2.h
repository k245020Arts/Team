#pragma once
#include "EnemyStateBase.h"

class BossNormalAttack2 : public EnemyStateBase
{
public:
	BossNormalAttack2();
	~BossNormalAttack2();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float averageSpeed;
};