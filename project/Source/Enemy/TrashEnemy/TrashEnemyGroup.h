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

	void SettingGroup(TrashEnemy* _enemy, int _index);

public:
	std::list<TrashEnemy*> rangedEnemies;
	bool hasLeader;
};