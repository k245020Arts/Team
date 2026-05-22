#pragma once
#include "T_EnemyAttack.h"

class CooperateAttack2 :public T_EnemyAttack
{
public:
	CooperateAttack2();
	~CooperateAttack2();

	void Update()override;

	void Start()override;
	void Finish()override;
private:
	void RangedMove(TrashEnemy* _enemy);
	void DamageMove(TrashEnemy* _enemy);

	const VECTOR3 CollPos	= VECTOR3(0.0f, 100.0f, 50.0f);
	const VECTOR3 Collscale = VECTOR3(100.0f, 0.0f, 0.0f);

	float speedDownCounter;

	bool damageMove;
	float hitStopCounter;
	
	VECTOR3 pPos;

	bool isDamageMove;

	float motionSpeed;

};