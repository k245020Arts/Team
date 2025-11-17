#pragma once
#include "../../../Enemy/EnemyState/EnemyStateBase.h"

class BossAttackBase:public EnemyStateBase
{
public:
	BossAttackBase();
	~BossAttackBase();

	float GetHitDamage() { return hitDamage; }
protected:
	float hitDamage;
	float motionSpeed;
	float motionMaxSpeed;
};