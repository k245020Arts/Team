#pragma once
#include "../../Boss/BossState/Attack/BossAttackBase.h"

class TrashEnemy;

class T_EnemyAttack :public BossAttackBase
{
public:
	T_EnemyAttack();
	~T_EnemyAttack();
	virtual void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

protected:
	//エフェクトなどの情報が入ってる
	void AttackInformation(TrashEnemy* _e);
private:
	float counter;
	float mSpeed;
	float mMaxFrame;
	//減速中か確認
	bool isDecel;
};