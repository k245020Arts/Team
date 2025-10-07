#pragma once
#include "CharaBase.h"
#include "enemyInformation.h"
#include "transform.h"
#include "enemyDamage.h"

class StateManager;
class EnemyStateManager;

class Enemy : public CharaBase
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

	void Start(Object3D* _obj);
	void ImguiDraw()override;

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

	void PlayerHit();
	void DrawTrail();

	Transform* GetEnemyTransform() { return enemyTransform; }
	BaseObject* GetEnemyObj() { return obj; }

	bool IsShake();
	bool PlayerPointerSet(BaseObject* _obj);

	StateManager* GetEnemyStateManager() { return eCom.state; }

	void EnemyDamageMove(EnemyDamage::EnemyDamageInfo _info);

private:
	EnemyInformation::EnemyComponent eCom;
	
	Transform* enemyTransform;
	StateManager* pState;
	float damageFlash;
};