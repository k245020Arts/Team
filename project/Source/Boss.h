#pragma once
#include "CharaBase.h"
#include"Object3D.h"
#include "BossInformation.h"

class Boss : public CharaBase
{
public:
	Boss();
	~Boss();
	void Update()override;
	void Draw()override;

	void Start(Object3D* _obj);

private:
	Boss* boss;

	BossInformation::BossComponent bossComponent;
	Transform* bossTransform;

};
