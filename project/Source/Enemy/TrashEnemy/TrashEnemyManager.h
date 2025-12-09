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
	/// 雑魚敵の生成
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="enemySpawnCounter">何体生成するか</param>
	void CreateEnemy(VECTOR3 _pos,float enemySpawnCounter);
	//敵のリストの取得
	std::list<TrashEnemy*>& EnemyList(){ return enemies; }

	void ImguiDraw();

	//連携攻撃
	void Cooperate(StateID::State_ID _id);
	//敵のステートを強制的に全員変える
	void AllChangeState(StateID::State_ID _id);

private:
	std::list<TrashEnemy*> enemies;
	Object3D* player;
	Object3D* camera;

	bool comboRequest;
	int counter;
};