#pragma once
#include "../../TrashEnemy/EnemyState/EnemyStateBase.h"
#include "../Boss.h"

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

	void NormalAttackSelect();
	int AttackPriority();

	float coolTime;

	Boss::HP_RATE hp;

	bool jump;
	int kind;
	int attackNum;
};