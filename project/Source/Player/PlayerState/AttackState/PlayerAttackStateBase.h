#pragma once
#include "../PlayerStateBase.h"
#include "../../../Common/ID/ID.h"

class PlayerAttackStateBase : public PlayerStateBase
{
public:

	PlayerAttackStateBase();
	~PlayerAttackStateBase();

	virtual void Update()override;

	virtual void EnemyRotation();
	void Start()override;
	void Finish() override;

	float GetHitDamage() { return hitDamage; }
	bool IsAttack();

protected:
	float easingCount;
	float beforeAngle;
	float angle;
	bool nextAttack;
	bool nextAvoid;
	Transform targetTrans;
	VECTOR3 dist;
	VECTOR3 norm;
	float distSize;
	float time;

	const float ATTACK_MOVE_DIST = 3000.0f;
	const float DISTANCE_MOVE = 800;

	bool beforeAttack;

	float runTimer;

	ID::IDType nextAttackID;
	float frontSpeed;

	const float ATTACK_FINISH_ANIM_SPEED = 0.1f;
	bool rotation;

	float hitDamage;
	bool defalutTrail;

};