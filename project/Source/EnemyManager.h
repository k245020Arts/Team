#pragma once
#include "../Library/GameObject.h"
#include <list>

class Object3D;

class EnemyManager : public GameObject
{
public:
	EnemyManager();
	~EnemyManager();
	void Update() override;
	void Draw() override;

	std::list<Object3D*> GetEnemy();

	void CreateEnemy();
	void PlayerObjPointer();

	void CreateBoss();

private:
	
	std::list<Object3D*> enemy;

};