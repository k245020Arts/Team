#pragma once
#include "PlayerAttackStateBase.h"

class Transform;

class PlayerJustAvoidAttack4 : public PlayerAttackStateBase
{
public:
	PlayerJustAvoidAttack4();
	~PlayerJustAvoidAttack4();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	
};