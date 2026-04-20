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

	/// <summary>
	/// ‹ß‹——£‚Æ‰“‹——£‚Ì“G‚ğ‹Ï“™‚É•ª‚¯‚é
	/// </summary>
	/// <param name="_enemy">“G‚ÌƒNƒ‰ƒX</param>
	/// <param name="_index">‰½‘Ì–Ú‚Ì“G‚©</param>
	void SettingGroup(TrashEnemy* _enemy, int _index);
	//‹ß‹——£‚Ì“G‚¾‚¯‚ğì‚é
	void SetMeleeEnemy(TrashEnemy* _enemy);
	//‰“‹——£‚Ì“G‚¾‚¯‚ğì‚é
	void SetRangedEnemy(TrashEnemy* _enemy);
	//¡¶‚«‚Ä‚é‚·‚×‚Ä‚Ì“G‚Ì”
	int GetEnemySize()const;

	//ˆê”Ô‹ß‚¢ƒEƒFƒCƒ|ƒCƒ“ƒg‚ğŒvZ‚·‚é
	void CloseWayPoint(std::vector<WayPoint> wayPoint);

public:
	Camera* camera;
	TrashEnemyManager* trashEnemyManager;

	//‹ß‹——£‚Ì“G
	std::list<TrashEnemy*> meleeEnemies;
	//‰“‹——£‚Ì“G
	std::list<TrashEnemy*> rangedEnemies;
	//
	std::list<TrashEnemy*> allEnemy;

	const int ATK_COUNTER_MIN = 1;
	const float ATK_COUNTER_MAX = 3;
	/// <summary>
	/// w’è‚µ‚½“G‚ÌƒŠƒXƒg‚Ì’†g‚ğŒ©‚Ä“G‚ª‚â‚ç‚ê‚Ä‚½‚çƒŠƒXƒg‚©‚çíœ‚·‚é
	/// </summary>
	/// <param name="enemies">“G‚ÌƒŠƒXƒg</param>
	void EnemyDeaad(std::list<TrashEnemy*>& enemies);
	//G‹›“I‚Ç‚¤‚µ‚Å‚ ‚½‚Á‚½‚Ì‰Ÿ‚µ•Ô‚µ
	void Separation();

	void InCameraWayPoint(WayPoint& _wayPoint);
	//¶‚«‚Ä‚é“G‚ğ”‚¦‚éŠÖ”
	int GetActiveEnemy();

	bool hasLeader;

	float separationTime;

	//‹ß‹——£‚Ì“G‚Åg‚¤----------
	//“G‚ÌUŒ‚
	void MeleeEnemyAttack(TrashEnemy* _enemy);
	//
	void EnemiesRun(TrashEnemy* _enemy);
	//‹ß‹——£‚Ì“G‚Ì˜AŒgUŒ‚
	void CooperateAttackMove(TrashEnemy* _enemy);
	//‹ß‹——£‚Ì“G‚ÌƒXƒe[ƒg‚ğ‹­§“I‚É‘Sˆõ•Ï‚¦‚é
	void AllChangeMeleeState(StateID::State_ID _id);
	
	float attackCounter;
	float maxAttackCounter;
	float standbyCounter;
	//˜AŒgUŒ‚‚Å‚±‚ÌƒJƒEƒ“ƒg‚ªˆê’è‚Ì’l‚ğ’´‚¦‚é‚Æ‹­§“I‚ÉUŒ‚‚µn‚ß‚é
	float cooperateCounter;

	int enemiesRunCounter;
	//--------------------------

	//‰“‹——£‚Ì“G----------------
	void RangedEnemyAttack(TrashEnemy* _enemy);

	//WayPoint RangedEnemyPoint;
	VECTOR3 leaderPos;
	//--------------------------
};