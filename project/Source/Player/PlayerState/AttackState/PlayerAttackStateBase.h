#pragma once
#include "../PlayerStateBase.h"

class PlayerAttackStateBase : public PlayerStateBase
{
public:

	PlayerAttackStateBase();
	virtual ~PlayerAttackStateBase();

	virtual void Update()override;

	/// <summary>
	/// Enemy‚Ì•ûŒü‚ÉŒü‚­
	/// </summary>
	virtual void EnemyRotation();
	virtual void Start()override;
	virtual void Finish() override;
	/// <summary>
	/// player‚ÌUŒ‚—Í‚ğæ“¾
	/// </summary>
	/// <returns></returns>
	float GetHitDamage()const { return hitDamage; }
	/// <summary>
	/// Œ»İUŒ‚’†‚©‚ğæ“¾
	/// </summary>
	/// <returns></returns>
	bool IsAttack();

	/// <summary>
	/// UŒ‚‚Ì“–‚½‚è”»’è‚Ì¶¬‚ğ‚ ‚é’ö“x“ˆê‰»‚³‚¹‚½ŠÖ”
	/// </summary>
	void AttackCollsion();
	/// <summary>
	/// “–‚½‚è”»’è‚ª’¼‘O‚É¶¬‚³‚ê‚½‚©‚ğæ“¾
	/// </summary>
	/// <returns>UŒ‚‚ª¶¬‚³‚ê‚Ä‚¢‚½‚çtrue</returns>
	bool GetCollsionCreate() { return collsionCreate; }

protected:
	float easingCount;
	float beforeAngle;
	float angle;
	bool nextAttack;
	bool nextAvoid;
	
	VECTOR3 dist;
	VECTOR3 norm;
	float distSize;
	float time;

	const float ATTACK_MOVE_DIST = 3000.0f;
	const float DISTANCE_MOVE = 800;

	bool beforeAttack;

	float runTimer;

	StateID::State_ID nextAttackID;
	float frontSpeed;

	const float ATTACK_FINISH_ANIM_SPEED = 0.1f;
	bool rotation;

	float hitDamage;
	bool defalutTrail;
	bool rockOn;

	int attackCount;
	float attackAgainStartCounter;
	float attackAgainStartCounterMax;

	int attackNum;
	bool collsionCreate;

	void BaseAttackCollsion();
	void AttackMoveStart();

	void AgainAttackCollsion();
	void AgainTimerSet(float _time, int _attackNum);
};