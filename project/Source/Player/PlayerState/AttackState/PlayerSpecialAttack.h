#pragma once
#include "PlayerAttackStateBase.h"

class PlayerSpecialAttack : public PlayerAttackStateBase
{
public:
	PlayerSpecialAttack();
	~PlayerSpecialAttack();

	void Update()override;
	void Draw()override;
	void Start()override;
	void Finish()override;

private:

};
