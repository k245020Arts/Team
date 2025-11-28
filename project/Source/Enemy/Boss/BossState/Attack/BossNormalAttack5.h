#pragma once
#include "BossAttackBase.h"

class BossNormalAttack5 : public BossAttackBase
{
public:
	BossNormalAttack5();
	~BossNormalAttack5();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float averageSpeed;
};