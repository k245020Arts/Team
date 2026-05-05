#pragma once
#include "../../../Library/GameObject.h"
#include <list>
#include "TrashEnemyManager.h"

class TrashEnemyManager;
class TrashEnemy;
class Camera;

class TrashEnemyGroup : public GameObject
{
public:
	TrashEnemyGroup();
	~TrashEnemyGroup();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// 近距離と遠距離の敵を均等に分ける
	/// </summary>
	/// <param name="_enemy">敵のクラス</param>
	/// <param name="_index">何体目の敵か</param>
	void SettingGroup(TrashEnemy* _enemy, int _index);
	//近距離の敵だけを作る
	void SetMeleeEnemy(TrashEnemy* _enemy);
	//遠距離の敵だけを作る
	void SetRangedEnemy(TrashEnemy* _enemy);

	//生きてるすべての敵を数える関数
	int GetActiveEnemy()const;
	//生きてる近距離の敵を数える関数
	int GetMeleeActiveEnemy()const;
	//生きてる遠距離の敵を数える関数
	int GetRangedActiveEnemy()const;

	//一番近いウェイポイントを計算する
	void CloseWayPoint(std::vector<WayPoint> wayPoint);
	//遠距離の敵の連携攻撃
	void RangedEnemyAttack();
	//連携攻撃中に攻撃を食らった後の処理
	void DeadMeleeEnemy();

private:
	Camera* camera;
	TrashEnemyManager* trashEnemyManager;

	//近距離の敵
	std::list<TrashEnemy*> meleeEnemies;
	//遠距離の敵
	std::list<TrashEnemy*> rangedEnemies;
	//
	std::list<TrashEnemy*> allEnemy;

	const int ATK_COUNTER_MIN = 1;
	const float ATK_COUNTER_MAX = 3;
	/// <summary>
	/// 指定した敵のリストの中身を見て敵がやられてたらリストから削除する
	/// </summary>
	/// <param name="enemies">敵のリスト</param>
	void EnemyDeaad(std::list<TrashEnemy*>& enemies);
	//雑魚的どうしであたった時の押し返し
	void Separation();

	void InCameraWayPoint(WayPoint& _wayPoint);
	
	bool hasLeader;

	//近距離の敵で使う----------
	//敵の攻撃
	void MeleeEnemyAttack(TrashEnemy* _enemy);
	//近距離の敵の走るポイントを決める
	void EnemiesRun(TrashEnemy* _enemy);
	//近距離の敵の連携攻撃
	void CooperateAttackMove(TrashEnemy* _enemy);
	//近距離の敵のステートを指定したステートに全員変える処理
	void AllChangeMeleeState(StateID::State_ID _id);
	//連携時に連携する敵を線でつなげる
	void CooperateAttackLine();
	
	float attackCounter;
	float maxAttackCounter;
	float standbyCounter;
	//連携攻撃でこのカウントが一定の値を超えると強制的に攻撃し始める
	float cooperateCounter;

	int enemiesRunCounter;

	//--------------------------

	//遠距離の敵----------------
	//敵の待機の挙動
	void RangedEnemySetWaypoint(TrashEnemy* _enemy);
	//連携攻撃を失敗したときの倒される処理
	void RangedDamageMove();
	//遠距離の敵のステートを指定したステートに全員変える処理
	void AllChangeRangedState(StateID::State_ID _id);

	VECTOR3 leaderPos;

	float rangedAtkTime;
	bool rangedDamageMove;
	int rangedAtkCounter;
	//連携開始時のリーダー以外の敵が何体いるか
	int rangedJoinCounter;

	//--------------------------
};