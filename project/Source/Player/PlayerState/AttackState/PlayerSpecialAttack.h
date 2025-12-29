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

	void MoveStart(float _angle);

	void AddCollsion();

	void StateImguiDraw()override;

private:

	float radius;
	
	int moveNum;
};
