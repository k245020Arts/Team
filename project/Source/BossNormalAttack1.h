#pragma once
#include "BossAttackBase.h"

class BossNormalAttack1 : public BossAttackBase
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