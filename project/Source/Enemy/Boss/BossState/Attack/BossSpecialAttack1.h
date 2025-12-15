#pragma once
#include "BossAttackBase.h"

class BossSpecialAttack1 :public BossAttackBase
{
public:
	BossSpecialAttack1();
	~BossSpecialAttack1();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:
	VECTOR3 velocity;
	VECTOR3 rotation;
	float counter;
	float copyPos;
	float count;
	float subSpeed;
	float attackStart;
	bool firstCount;
	bool effect;
};