#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class AttackSorting;
class BossStatus;

class BossRun : public EnemyStateBase
{
public:
	BossRun();
	~BossRun();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	BossStatus* bs;
	VECTOR3 velocity;
	VECTOR3 rotation;

	VECTOR3 targetPos;
	//岩をよける
	void HitLine();
	//近くの岩のポジションを入れる
	std::list<VECTOR3> RayCastHits();
	float speed;

	bool islooping;
	float lineCounter;

	float atkTransferCounter;
	//攻撃に移行する最大時間
	const float MAX_TRANSFER_ATK = 5.0f;
};