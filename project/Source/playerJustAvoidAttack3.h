#pragma once
#include "PlayerAttackStateBase.h"

class Transform;

class PlayerJustAvoidAttack3 : public PlayerAttackStateBase
{
public:
	PlayerJustAvoidAttack3();
	~PlayerJustAvoidAttack3();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	void Again();

private:
	float timer;
	int count;

	const int ATTACK_NUMMAX = 2;
};