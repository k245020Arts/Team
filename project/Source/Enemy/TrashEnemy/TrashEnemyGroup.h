#pragma once
#include "../../../Library/GameObject.h"
#include <list>
#include "TrashEnemyManager.h"

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
	//‹ß‹——£‚Ì“G
	std::list<TrashEnemy*> meleeEnemies;
	//‰“‹——£‚Ì“G
	std::list<TrashEnemy*> rangedEnemies;

	const int ATK_COUNTER_MIN = 1;
	const float ATK_COUNTER_MAX = 3;
	
	void EnemyDeaad(std::list<TrashEnemy*>& enemies);
	//G‹›“I‚Ç‚¤‚µ‚Å‚ ‚½‚Á‚½‚Ì‰Ÿ‚µ•Ô‚µ
	void Separation();
	//¶‚«‚Ä‚é“G‚ğ”‚¦‚éŠÖ”
	int GetActiveEnemy();

	bool hasLeader;

	//‹ß‹——£‚Ì“G‚Åg‚¤----------
	//“G‚ÌUŒ‚
	void MeleeEnemyAttack(TrashEnemy* _enemy);
	//‹ß‹——£‚Ì“G‚Ì˜AŒgUŒ‚
	void CooperateAttackMove(TrashEnemy* _enemy);
	//‹ß‹——£‚Ì“G‚ÌƒXƒe[ƒg‚ğ‹­§“I‚É‘Sˆõ•Ï‚¦‚é
	void AllChangeState(StateID::State_ID _id);
	
	float attackCounter;
	float maxAttackCounter;
	float standbyCounter;
	//˜AŒgUŒ‚‚Å‚±‚ÌƒJƒEƒ“ƒg‚ªˆê’è‚Ì’l‚ğ’´‚¦‚é‚Æ‹­§“I‚ÉUŒ‚‚µn‚ß‚é
	float cooperateCounter;

	//--------------------------

	//‰“‹——£‚Ì“G‚ÌUŒ‚
	void RangedEnemyAttack(TrashEnemy* _enemy);
};