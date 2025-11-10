#pragma once
#include "PlayerAttackStateBase.h"
#include "../Library/myDxLib.h"

class PlayerJustAvoidAttack5 : public PlayerAttackStateBase
{
public:
	PlayerJustAvoidAttack5();
	~PlayerJustAvoidAttack5();


	void Update()override;
	void Draw()override;
	void Start()override;
	void Finish()override;

private:
	float timer;
	bool oneEffect;
	float startTimer;
	bool attack;

	const VECTOR3 INTEREA = VECTOR3(2500.0f, 2500.0f, 2500.0f);
};