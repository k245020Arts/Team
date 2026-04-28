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
	void LeaderMove(TrashEnemy* _enemy);

	bool isLeader;
	
	VECTOR3 setGravity;
	bool damageMove;

	float speedDownCounter;
};