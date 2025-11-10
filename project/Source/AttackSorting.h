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
	const int MAXATK = 3;
	const float COOLTIME = 30.0f;
	float coolTime;
};