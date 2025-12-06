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

	void CreateTrashEnemy(VECTOR3 _pos);
	bool GetActive() { return active; }

	void Trail();

	void PlayerHit()override;

	//targetPosを変える関数
	void SetTargetPos(VECTOR3 _pos, StateID::State_ID _id);
	//敵のステートを変える関数
	void ChangeState(StateID::State_ID _id);
	VECTOR3 GetPos() { return obj->GetTransform()->position; }

	bool GetStandby() { return isStandby; }

	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, Transform _trans)
	{
		if (attackColl == nullptr)
		{
			CollsionInfo info = CharaBase::CollsionInstant<T>(_shape, _trans);
			info.tag = CollsionInformation::Tag::E_ATTACK;
			//collName = "e_attack"
			attackColl->CollsionAdd(info, _trans, "e_attack");
		};
		return static_cast<T*>(attackColl);
	}

private:
	CharaWeapon* chara;
	T_EnemyStatus* eStatus;

	StateID::State_ID attackId;

	//targetPosに入ってる方向に向く
	void LookTarget();
	bool active;
	//どこに向くか
	VECTOR3 targetPos;
	//連携攻撃してるか
	bool isStandby;
};