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

	void SpecialRotationChange();

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
	VECTOR3 defalutRotation;
	VECTOR3 keepPos;
	VECTOR3 dir;
	float specialAngle;

	float currentAngle;
	float angleMin;
	float angleMax;
	float randAngle;

	VECTOR3 lineStart;
	VECTOR3 lineEnd;

	float moveT;
	float moveSpeed; 
	bool centerTo;

	int boxHandle;
};
