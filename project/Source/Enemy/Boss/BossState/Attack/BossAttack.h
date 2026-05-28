#pragma once
#include "BossAttackBase.h"

class BossAttack : public BossAttackBase
{
public:

	BossAttack();
	~BossAttack();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};
