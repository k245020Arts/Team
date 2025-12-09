#pragma once
#include "../../../TrashEnemy/EnemyState/EnemyStateBase.h"

class BossAttackBase:public EnemyStateBase
{
public:

	enum PlayerDamagePattern
	{
		NONE = -1,
		NO_BACK = 0,
		BACK,
		BLOW_AWAY,
	};


	struct DamagePattern
	{
		float hitDamage;
		float motionSpeed;
		float motionMaxSpeed;
		PlayerDamagePattern damagePattern;
		bool flash;
	};

	
	BossAttackBase();
	~BossAttackBase();

	void BossStart();
	void BossFinish();

	float GetHitDamage() { return damage.hitDamage; }
	const DamagePattern& GetDamageParam() { return damage; }
protected:
	DamagePattern damage;
};