#pragma once
#include "../../../Library/GameObject.h"
#include "../EnemyBase.h"
#include <list>

class TrashEnemy;
class Object3D;
class Camera;

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
	const int ENEMIESMAX = 30;
	const int ATK_COUNTER_MIN = 3;
	const int ATK_COUNTER_MAX = 5;
	
	struct WayPoint
	{
		VECTOR3 position;
		bool active;
		WayPoint()
		{
			position = VZero;
			active = false;
		}
		WayPoint(VECTOR3 _pos, bool _active)
		{
			position = _pos;
			active = _active;
		}
	};
	//ウェイポイントの元を保管する変数
	std::list<VECTOR3> wayPointOffsets;
	//ウェイポイントを保管する変数
	std::vector<WayPoint> wayPoint;

	Object3D* player;
	Camera* camera;
	//雑魚敵の通常攻撃の処理
	void NormalAttackMove(TrashEnemy* _enemy);
	//連携攻撃の処理
	void CooperateAttackMove(TrashEnemy* _enemy);
	//ウェイポイントを最初に作る
	void WayPointOffset();
	//プレイヤーの周りにポイントを作る
	void PlayerWayPoint();
	//一番近いウェイポイントを計算する
	void CloseWayPoint();

	bool comboRequest;
	//int counter;
	float attackCounter;
	int maxAttackCounter;
	int standbyCounter;

	float searchCounter;
	//連携攻撃でこのカウントが一定の値を超えると強制的に攻撃し始める
	float cooperateCounter;
	bool debugWaypoint;
};