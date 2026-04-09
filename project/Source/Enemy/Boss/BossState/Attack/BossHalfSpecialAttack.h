#pragma once
#include "BossAttackBase.h"

class BossHalfSpecialAttack : public BossAttackBase
{
public:
	BossHalfSpecialAttack();
	~BossHalfSpecialAttack();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;


private:
	/*float rockCreateCounter;
	bool oneCreate;
	int totalRocks;
	int rocksDropped;
	float interval;
	bool lastRockCreated;
	float timer;

	float rotateAngle;
	float rotateSpeed;

	bool chargingLast;
	float chargeTimer;*/
};