#pragma once
#include "../../../Library/GameObject.h"
#include "../EnemyBase.h"
#include <list>

class TrashEnemy;
class Object3D;
class TrashEnemyGroup;

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
	void CreateEnemy(VECTOR3 _pos, int meleeSpawnCounter, int rangedSpawnCounter);

	//敵が何人生きているかわかる関数
	int GetActiveEnemy()const;
	//生きてる近距離の敵を数える関数
	int GetMeleeActiveEnemy()const;
	//生きてる遠距離の敵を数える関数
	int GetRangedActiveEnemy()const;

	//遠距離の連携を始めるか終えることができる
	void SetStartRangedAttack(bool _endAtk) { startRangedAtk = _endAtk; }
	void ImguiDraw();

	//連携攻撃
	void Cooperate();
	
	VECTOR3 GetPlayerPos() { return player->GetTransform()->position; }
	std::vector<VECTOR3> GetWayPointPosition();
private:
	const int ENEMIESMAX = 30;
	const int ATK_COUNTER_MIN = 1;
	const float ATK_COUNTER_MAX = 3;

	TrashEnemyGroup* enemyGroup;
	//敵のモデルを保持する
	/*std::vector<int> hModels;

	std::vector<std::pair<std::string, std::string>> charaTypeID;
	std::vector <std::string> charaID;
	std::vector <std::string> typeID;*/
	struct EnemyResource
	{
		std::string charaID;
		std::string typeID;
		std::string charaTypeID;
		int model;
		EnemyResource(std::string _charaID, std::string _typeID, int _model)
		{
			charaID = _charaID;
			typeID = _typeID;
			model = _model;

			charaTypeID = charaID + typeID;
		}
	};
	std::vector<EnemyResource> resources;
	/// <summary>
	/// 敵の元データを作る
	/// </summary>
	/// <param name="_charaID"></param>
	/// <param name="_typeID">雑魚敵の種類のID</param>
	/// <param name="_i"></param>
	void CreateData(EnemyResource _resource,int _i);


	//ウェイポイントの元を保管する変数
	std::list<VECTOR3> wayPointOffsets;
	//ウェイポイントを保管する変数
	std::vector<WayPoint> wayPoint;

	Object3D* player;
	//std::list<Object3D*> stage;
	Object3D* stage[4];
	//Camera* camera;
	
	//ウェイポイントを最初に作る
	void WayPointOffset();
	//プレイヤーの周りにポイントを作る
	void PlayerWayPoint();
	
	bool comboRequest;
	//int counter;
	
	float maxAttackCounter;//
	int standbyCounter;

	float searchCounter;
	
	bool debugWaypoint;

	bool startRangedAtk;
	TrashEnemy* trashEnemy;

	VECTOR3 setPos;
	//何種類目の敵か
	int numTypeCounter;
	//どのポイントを追いかけるかを決める
	int runPoint;
};