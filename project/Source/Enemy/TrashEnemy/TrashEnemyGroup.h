#pragma once
#include "../../../Library/GameObject.h"
#include <list>

class TrashEnemy;

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

public:
	std::list<TrashEnemy*> meleeEnemies;
	std::list<TrashEnemy*> rangedEnemies;
	bool hasLeader;
};