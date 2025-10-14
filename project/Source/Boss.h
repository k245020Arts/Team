#pragma once
#include "EnemyBase.h"
#include"Object3D.h"
#include "BossInformation.h"

class Boss : public EnemyBase
{
public:
	friend class BossIdol;
	Boss();
	~Boss();
	void Update()override;
	void Draw()override;

	void Start(Object3D* _obj);

private:
	Boss* boss;

	//BossInformation::BossComponent bossComponent;
	Transform* bossTransform;

};
