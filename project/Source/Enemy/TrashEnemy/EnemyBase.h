#pragma once
#include "../CharaBase/CharaBase.h"
#include "enemyInformation.h"
#include "../Enemy/EnemyState/EnemyDamage.h"
#include "../Common/ID/EffectID.h"

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
	bool GetHit() { return hit; }

	

protected:

	EnemyInformation::EnemyComponent enemyBaseComponent;
	float damageFlash;
	float hitCounter;
	int loopNum;
	StateManager* pState;

	
	const float MAX_HP = 100;
	bool hit;
};
