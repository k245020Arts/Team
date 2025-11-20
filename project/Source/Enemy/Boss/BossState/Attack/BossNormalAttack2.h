#pragma once
#include "BossAttackBase.h"

class BossNormalAttack2 : public BossAttackBase
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