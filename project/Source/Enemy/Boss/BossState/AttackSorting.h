#pragma once
#include "../../TrashEnemy/EnemyState/EnemyStateBase.h"

class AttackSorting :public EnemyStateBase
{
public:
	
	enum HP_RATE
	{
		MAX,
		EIGHT,
		FIVE,
		THREE,
	};

	AttackSorting();
	~AttackSorting();
	void Update()override;

	void Start()override;
	void Finish()override;

private:
	const float COOLTIME = 30.0f;

	HP_RATE Hp();
	void RandomAttack();

	void NormalAttackSelect();

	float coolTime;

	HP_RATE hp;

	bool jump;
	int kind;
	int attackNum;
};