#pragma once
#include "CharaBase.h"
#include "enemyInformation.h"

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

protected:

	EnemyInformation::EnemyComponent enemyBaseComponent;
	float damageFlash;
	float hitCounter;
	int loopNum;

};
