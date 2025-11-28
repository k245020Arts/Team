#pragma once
#include "BossAttackBase.h"

class BossNormalAttack4 : public BossAttackBase
{
public:
	BossNormalAttack4();
	~BossNormalAttack4();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float averageSpeed;
};