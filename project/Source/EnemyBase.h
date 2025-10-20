#pragma once
#include "CharaBase.h"
#include "enemyInformation.h"
#include "enemyDamage.h"

class EnemyBase : public CharaBase
{
public:
	friend class EnemyStateBase;
	EnemyBase();
	~EnemyBase();

	void Update()override;
	void Draw()override;

	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, Transform _trans) {
		return nullptr;
	}

	StateManager* GetStateManager() { return enemyBaseComponent.state; }
	BaseObject* GetEnemyObj() { return obj; }

	virtual void PlayerHit() { ; }
	void DrawTrail();

	void EnemyDamageMove(EnemyDamage::EnemyDamageInfo _info);

protected:

	EnemyInformation::EnemyComponent enemyBaseComponent;
	float damageFlash;
	float hitCounter;
	int loopNum;
	StateManager* pState;

	const float HIT_EFFECT_TIME = 0.2f;
	const float HIT_EFFECT_SCALE_RATE = 0.1f;
	const float MAX_HP = 100;
};
