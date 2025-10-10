#pragma once
#include "../Library/GameObject.h"
#include "Object3D.h"

class BossManager : public GameObject
{
public:
	BossManager();
	~BossManager();

	void CreateBoss();

private:
	Object3D* boss;
};