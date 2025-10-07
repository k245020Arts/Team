#pragma once
#include "../Library/GameObject.h"

class Object3D;

class EnemyManager : public GameObject
{
public:
	EnemyManager();
	~EnemyManager();
	void Update() override;
	void Draw() override;

	Object3D* GetEnemy();

	void CreateEnemy();
	void PlayerObjPointer();

private:
	
	Object3D* enemy;

};