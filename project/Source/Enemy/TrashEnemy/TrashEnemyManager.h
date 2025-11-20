#pragma once
#include "../../../Library/GameObject.h"
#include "../EnemyBase.h"
#include <list>

class TrashEnemy;

class TrashEnemyManager : public GameObject
{
public:
	TrashEnemyManager();
	~TrashEnemyManager();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// G‹›“G‚Ì¶¬
	/// </summary>
	/// <param name="_pos">À•W</param>
	/// <param name="enemySpawnCounter">‰½‘Ì¶¬‚·‚é‚©</param>
	void CreateEnemy(VECTOR3 _pos,float enemySpawnCounter);
	//“G‚ÌƒŠƒXƒg‚Ìæ“¾
	std::list<TrashEnemy*>& EnemyList(){ return enemies; }

private:
	std::list<TrashEnemy*> enemies;

};