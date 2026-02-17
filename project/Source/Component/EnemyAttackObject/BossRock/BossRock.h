#pragma once
#include "../EnemyAttackObject.h"
#include "../../Collider/CollsionInformation.h"

class SphereCollider;
class RayCollider;

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

private:
	RayCollider* randColl;
	SphereCollider* playerHitColl;
	SphereCollider* bossHitColl;

	CollsionInformation::Tag collTag;

	float time;
};
