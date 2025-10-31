#pragma once
#include "EnemyStateBase.h"

class BossNormalAttack3 : public EnemyStateBase
{
public:
	BossNormalAttack3();
	~BossNormalAttack3();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	int counter;
};