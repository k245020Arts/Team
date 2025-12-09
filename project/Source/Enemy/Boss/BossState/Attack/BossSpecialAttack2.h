#pragma once
#include "BossAttackBase.h"

class BossSpecialAttack2 : public BossAttackBase
{
public:
	BossSpecialAttack2();
	~BossSpecialAttack2();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float attackCount;
	VECTOR3 rotation;
	float distance;
	bool look;
	void AttackStart();

	float turningTime;
};
