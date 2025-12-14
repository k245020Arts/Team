#pragma once
#include "../../../Library/GameObject.h"
#include "../EnemyBase.h"
#include <list>

class TrashEnemy;
class Object3D;

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

	void ImguiDraw();

	//˜AŒgUŒ‚
	void Cooperate(StateID::State_ID _id);
	//“G‚ÌƒXƒe[ƒg‚ğ‹­§“I‚É‘Sˆõ•Ï‚¦‚é
	void AllChangeState(StateID::State_ID _id);
	//G‹›“I‚Ç‚¤‚µ‚Å‚ ‚½‚Á‚½‚Ì‰Ÿ‚µ•Ô‚µ
	void Separation();

private:
	std::list<TrashEnemy*> enemies;
	Object3D* player;
	Object3D* camera;

	VECTOR3 C_Attack1Pos(int index);

	bool comboRequest;
	int counter;
	int attackCounter;
	int standbyCounter;
};