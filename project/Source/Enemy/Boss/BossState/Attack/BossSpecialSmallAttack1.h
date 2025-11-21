#pragma once
#include "BossAttackBase.h"

class BossSpecialSmallAttack1 : public BossAttackBase
{
public:
	 BossSpecialSmallAttack1();
	~ BossSpecialSmallAttack1();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float subSpeed;
	float attackStart;
	bool firstCount;
	bool effect;
};