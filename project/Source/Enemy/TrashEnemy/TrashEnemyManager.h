#pragma once
#include "../../../Library/GameObject.h"

class TrashEnemyManager : public GameObject
{
public:
	TrashEnemyManager();
	~TrashEnemyManager();

	void Update()override;
	void Draw()override;

	void CreateEnemy();
private:

};