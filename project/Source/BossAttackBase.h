#pragma once
#include "EnemyStateBase.h"

class BossAttackBase:public EnemyStateBase
{
public:
	BossAttackBase();
	~BossAttackBase();

	float GetHitDamage() { return hitDamage; }
protected:
	float hitDamage;
};