#pragma once
#include "../EnemyAttackObject.h"
#include "../../Collider/CollsionInformation.h"
#include <unordered_set>

class SphereCollider;
class RayCollider;
class ModelCollider;
class BossRockManager;
class DountCollider;
class SoundManager;
class EffectManager;

class BossRockBase : public EnemyAttackObject
{
public:
	 BossRockBase();
	virtual ~BossRockBase();
	void Awake()override;

	virtual void Update()override;

	bool HitObjects(BaseObject* _obj) {
		return (hitObjects.count(_obj) > 0);
	}

	virtual void Ground();

	void AddHitObj(BaseObject* _obj) { hitObjects.insert(_obj); }
	void SetCanBlast(bool _blast) { blastCan = _blast; }
	void PlayerAttackRockFlyAway(Transform& _playerTransform);

	void SetRockModel();

	void RockBossHit();

protected:

	RayCollider* randColl;
	SphereCollider* playerHitColl;
	SphereCollider* bossHitColl;
	SphereCollider* playerAttackHitColl;
	SphereCollider* bossRushHitColl;
	DountCollider* blastColl;
	RayCollider* uiColl;
	ModelCollider* pushColl;
	SphereCollider* justAvoidCollider;

	SoundManager* soundManager;
	EffectManager* effectManager;
	Physics* physics;
	BossRockManager* rockManager;


	bool blast;
	Color* color;
	float blinkCounter;
	float blinkBaseMax;
	bool ChangeColorMode;

	bool nowBlast;

	std::unordered_set<BaseObject*> hitObjects;
	bool blastCan;
//private:
	bool groundInit;
	bool fly;
	int useHandleNumber;
};