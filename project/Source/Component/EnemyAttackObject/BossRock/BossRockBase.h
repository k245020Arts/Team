#pragma once
#include "../EnemyAttackObject.h"
#include <unordered_set>
#include "../../../Enemy/Boss/BossState/Attack/BossAttackBase.h"

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
	void Draw()override;

	bool HitObjects(BaseObject* _obj) {
		return (hitObjects.count(_obj) > 0);
	}

	virtual void Ground();

	void AddHitObj(BaseObject* _obj) { hitObjects.insert(_obj); }
	void SetCanBlast(bool _blast) { blastCan = _blast; }
	void PlayerAttackRockFlyAway(Transform& _playerTransform);

	void SetRockModel();

	void RockBossHit();

	void Start(const BossAttackBase::ThrowObjectAttackData& _attack);
	void DropObject();
	void ThrowRockStart(BaseObject* _player);
	const BossAttackBase::ThrowObjectAttackData& GetThrowObjectsData() { return attackData; }

	void SetPreInfo(const VECTOR3& _pos);

protected:

	RayCollider* randColl;
	SphereCollider* playerHitColl;
	SphereCollider* bossHitColl;
	SphereCollider* playerAttackHitColl;
	SphereCollider* bossRushHitColl;
	DountCollider* blastColl;
	DountCollider* blastJustAvoidColl;
	RayCollider* uiColl;
	ModelCollider* pushColl;
	SphereCollider* justAvoidCollider;

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
	BossAttackBase::ThrowObjectAttackData attackData;

	void BlastCollsionCreate();
	bool preDraw;
	Transform preTransform;
	int preModel;

	bool throwRock;
	VECTOR3 dir;
	int throwRockBaseModel;
	int boneNum;
	bool throwReady;
	VECTOR3 addPos;
	float groundTime;
	float flyCounter;
	bool playerAttackHit;
	bool velocityAdd;
};