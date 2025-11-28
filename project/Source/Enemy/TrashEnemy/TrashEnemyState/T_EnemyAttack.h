#pragma once
#include "../../Boss/BossState/Attack/BossAttackBase.h"

class T_EnemyAttack :public BossAttackBase
{
public:
	T_EnemyAttack();
	~T_EnemyAttack();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};