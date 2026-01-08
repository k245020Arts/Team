#pragma once
#include "PlayerAttackStateBase.h"

class PlayerSpecialAttack : public PlayerAttackStateBase
{
public:
	enum PLAYER_SPECIAL_ATTACK_STATE
	{
		NO_ATTACK = -1,
		BEFORE,
		GROUND_ATTACK,
		CHARGE,
		FINAL_ATTACK,
	};
	PlayerSpecialAttack();
	~PlayerSpecialAttack();

	void Update()override;
	void Draw()override;
	void Start()override;
	void Finish()override;

	void MoveStart(float _angle);

	void AddCollsion();

	void StateImguiDraw()override;

	void BeforeUpdate();
	void GroundUpdate();
	void ChargeUpdate();
	void FinalAttackUpdate();

private:

	float radius;
	
	int moveNum;

	PLAYER_SPECIAL_ATTACK_STATE state;
	float chargeCounter;
	float waitCounter;
};
