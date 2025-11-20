#pragma once
#include "BossAttackBase.h"

class BossNormalAttack3 : public BossAttackBase
{
public:
	BossNormalAttack3();
	~BossNormalAttack3();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	int counter;
};