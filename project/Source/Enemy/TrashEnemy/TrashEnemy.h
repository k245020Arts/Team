#pragma once
#include "../EnemyBase.h"
#include "../../Component/Object/Object3D.h"
#include "../EnemyInformation.h"

class StateManager;
class EnemyStateManager;
class CharaWeapon;
class T_EnemyStatus;

class TrashEnemy : public EnemyBase
{
public:
	friend class T_EnemyIdol;
	friend class T_EnemyRun;
	friend class T_EnemyAttack;
	friend class T_EnemyDead;
	friend class Standby;
	friend class CooperateAttack1;

	TrashEnemy();
	~TrashEnemy();

	void Update()override;
	void Draw()override;

	void Start(Object3D* _obj);

	void CreateTrashEnemy(VECTOR3 _pos,int _number);
	bool GetActive() { return active; }

	void Trail();

	void PlayerHit()override;

	//targetPosを変える関数
	void SetTargetPos(VECTOR3 _pos, StateID::State_ID _id);
	//敵のステートを変える関数
	void ChangeState(StateID::State_ID _id);
	VECTOR3 GetPos() { return obj->GetTransform()->position; }

	bool GetStandby() { return isStandby; }
	int GetNumber() { return number; }
	float Speed() { return speed; }

	void isStandbyF() { isStandby = false; }
	bool IsAttack() { return isAttack; }
	//攻撃命令
	void AttackCommand();
	//攻撃クールタイムリセット
	void AttackCoolTimeReset() { isAttack = true; }

	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, Transform _trans)
	{
		if (attackColl == nullptr)
		{
			CollsionInfo info = CharaBase::CollsionInstant<T>(_shape, _trans);
			info.tag = CollsionInformation::Tag::E_ATTACK;
			collName = "e_attack";
			attackColl->CollsionAdd(info, _trans, collName);
		};
		return static_cast<T*>(attackColl);
	}

	void Move(float _speed, float _max);

	void AddPos(VECTOR3 _pos);
private:
	CharaWeapon* chara;
	T_EnemyStatus* eStatus;

	//StateID::State_ID nextId;

	int number;

	//targetPosに入ってる方向に向く
	void LookTarget();
	bool active;
	//どこに向くか
	VECTOR3 targetPos;
	//通常攻撃をしてもよいか
	bool isAttack;
	//連携攻撃かどうか
	bool isCooperateAtk;
	//連携攻撃の準備ができてるか
	bool isStandby;
	
	float speed;

	VECTOR3 wayPoint;
};