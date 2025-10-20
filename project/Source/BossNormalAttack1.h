#pragma once
#include "EnemyStateBase.h"

class BossNormalAttack1 : public EnemyStateBase
{
public:
	BossNormalAttack1();
	~BossNormalAttack1();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float averageSpeed;
};