#pragma once
#include "../../Enemy/EnemyState/EnemyStateBase.h"

class BossSpecialAttack1 :public EnemyStateBase
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
	int a;
	float counter;
	float copyPos;
};