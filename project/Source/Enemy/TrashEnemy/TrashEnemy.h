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

	//targetPos‚ğ•Ï‚¦‚éŠÖ”
	void SetTargetPos(VECTOR3 _pos, StateID::State_ID _id);
	//“G‚ÌƒXƒe[ƒg‚ğ•Ï‚¦‚éŠÖ”
	void ChangeState(StateID::State_ID _id);
	VECTOR3 GetPos() { return obj->GetTransform()->position; }

	bool GetStandby() { return isStandby; }
	int GetNumber() { return number; }
	float Speed() { return speed; }

	void isStandbyF() { isStandby = false; }
	void AttackON() { isAttack = true; }
	void AddAttackID(int index);

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
private:
	CharaWeapon* chara;
	T_EnemyStatus* eStatus;

	StateID::State_ID NextId;

	int number;

	//targetPos‚É“ü‚Á‚Ä‚é•ûŒü‚ÉŒü‚­
	void LookTarget();
	bool active;
	//‚Ç‚±‚ÉŒü‚­‚©
	VECTOR3 targetPos;
	//’ÊíUŒ‚‚ğ‚µ‚Ä‚à‚æ‚¢‚©
	bool isAttack;
	//˜AŒgUŒ‚‚ğ‚·‚é‚©‚µ‚È‚¢‚©
	bool isCooperateAtk;
	//˜AŒgUŒ‚‚Ì€”õ‚ª‚Å‚«‚Ä‚é‚©
	bool isStandby;

	float speed;
};