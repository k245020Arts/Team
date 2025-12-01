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

	TrashEnemy();
	~TrashEnemy();

	void Update()override;
	void Draw()override;

	void Start(Object3D* _obj);

	void CreateTrashEnemy(VECTOR3 _pos);
	VECTOR3 GetPos() { return obj->GetTransform()->position; }
	bool GetActive() { return active; }

	void Trail();

	void PlayerHit()override;

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
	void LookPlayer();
	bool active;
};