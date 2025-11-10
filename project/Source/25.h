#pragma once
#include "PlayerAttackStateBase.h"

class Transform;

class PlayerJustAvoidAttack1 : public PlayerAttackStateBase
{
public:
	PlayerJustAvoidAttack1();
	~PlayerJustAvoidAttack1();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	void Again();


private:
	int count;
	float animSpeed;

	const int MAX_ATTACKNUM = 5;
	float timer;
};