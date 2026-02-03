#pragma once
#include "PlayerAttackStateBase.h"

class Transform;

class PlayerAttack5 : public PlayerAttackStateBase
{
public:
	PlayerAttack5();
	~PlayerAttack5();

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