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
	//雑魚的どうしであたった時の押し返し
	void Separation();

private:
	std::list<TrashEnemy*> enemies;
	//連携攻撃時の決めたポジションを保管する変数
	std::list<VECTOR3> savePos;
	Object3D* player;
	Object3D* camera;

	void SavePos();
	VECTOR3 C_Attack1Pos(VECTOR3 _pos);

	bool comboRequest;
	//int counter;
	int attackCounter;
	int standbyCounter;
};