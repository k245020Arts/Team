#pragma once
#include "EnemyBase.h"
#include "enemyInformation.h"
#include "../Component/Transform/transform.h"
#include "../Common/ID/SoundID.h"


class StateManager;
class EnemyStateManager;

class Enemy : public EnemyBase
{
public:
	friend class EnemyIdol;
	friend class EnemyAttack1;
	friend class EnemyDamage;
	friend class EnemyBlowAway;
	friend class EnemyRun;
	friend class EnemyDie;
	friend class EnemyStateBase;
	Enemy();
	~Enemy();

	void Update()override;
	void Draw()override;

	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, Transform _trans) {
		if (attackColl == nullptr) {
			CollsionInfo info = CharaBase::CollsionInstant<T>(_shape, _trans);
			info.tag = CollsionInformation::Tag::E_ATTACK;
			//collName = "e_attack"
			attackColl->CollsionAdd(info, _trans, "e_attack");
		};
		return static_cast<T*>(attackColl);
	}

	void Start(Object3D* _obj);
	void ImguiDraw()override;

	void PlayerHit()override;
	

	Transform* GetEnemyTransform() { return enemyTransform; }
	

	bool IsShake();
	bool PlayerPointerSet(BaseObject* _obj);
	
	
private:
	
	Transform* enemyTransform;
	

};