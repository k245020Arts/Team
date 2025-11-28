#pragma once
#include "BossAttackBase.h"

class BossNormalAttack6 : public BossAttackBase
{
public:
	BossNormalAttack6();
	~BossNormalAttack6();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float averageSpeed;
};