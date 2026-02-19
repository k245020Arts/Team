#pragma once
#include "../EnemyAttackObject.h"
#include "../../Collider/CollsionInformation.h"
#include "../../Transform/Transform.h"

class SphereCollider;
class RayCollider;
class ModelCollider;

class BossRock : public EnemyAttackObject
{
public:
	BossRock();
	~BossRock();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// 当たり判定の開始
	/// </summary>
	/// <param name="_tag">当たり判定のタグ</param>
	/// <param name="_transform">親にするトランスフォーム</param>
	void StartCollAdd(CollsionInformation::Tag _tag, const Transform& _transform);
	/// <summary>
	/// 当たり判定の取り除き
	/// </summary>
	/// <param name="_name">当たり判定のタグ///</param>
	void RemoveCollider(std::string _name);

	void SetRockModel();

	void SetPreInfo(const VECTOR3& _pos);

	void Ground();
	void PlayerAttackRockFlyAway(Transform& _playerTransform);

private:
	RayCollider* randColl;
	SphereCollider* playerHitColl;
	SphereCollider* bossHitColl;
	SphereCollider* playerAttackHitColl;
	RayCollider* uiColl;
	ModelCollider* pushColl;

	CollsionInformation::Tag collTag;

	float time;
	bool preDraw;
	Transform preTransform;
	int preModel;

	bool groundInit;
	int useHandleNumber;
	Physics* physics;
	bool fly;
};
