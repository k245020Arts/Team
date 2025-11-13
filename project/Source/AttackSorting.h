#pragma once
#include "enemyStateBase.h"

class AttackSorting :public EnemyStateBase
{
public:
	AttackSorting();
	~AttackSorting();
	void Update()override;

	void Start()override;
	void Finish()override;

private:
	const float COOLTIME = 30.0f;

	bool Hp();
	void RandomAttack();

	float coolTime;
};