#pragma once
#include "../../Boss/BossState/Attack/BossAttackBase.h"

class CooperateAttack1 :public BossAttackBase
{
public:
	CooperateAttack1();
	~CooperateAttack1();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:

};