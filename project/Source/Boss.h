#pragma once
#include "EnemyBase.h"
#include"Object3D.h"
#include "BossInformation.h"

class Player;

const float a =5000;

class Boss : public EnemyBase
{
public:
	friend class BossIdol;
	friend class BossRun;
	Boss();
	~Boss();
	void Update()override;
	void Draw()override;

	void Start(Object3D* _obj);

	//ƒvƒŒƒCƒ„[‚Ì•û‚ÉŒü‚­ŠÖ”
	void LookPlayer();

private:
	Boss* boss;
	
	//BossInformation::BossComponent bossComponent;
	Transform* bossTransform;

};
