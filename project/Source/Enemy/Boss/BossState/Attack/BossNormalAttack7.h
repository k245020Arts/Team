#pragma once
#include "BossAttackBase.h"

class BossNormalAttack7 : public BossAttackBase
{
public:
	BossNormalAttack7();
	~BossNormalAttack7();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	bool throwRock;
	bool rockGet;
};