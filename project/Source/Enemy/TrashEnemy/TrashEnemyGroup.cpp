#include "TrashEnemyGroup.h"
#include "TrashEnemy.h"

TrashEnemyGroup::TrashEnemyGroup()
{
	hasLeader = false;
}

TrashEnemyGroup::~TrashEnemyGroup()
{
}

void TrashEnemyGroup::Update()
{
}

void TrashEnemyGroup::SettingGroup(TrashEnemy* _enemy, int _index)
{
	if (_index % 2 == 0)
		_enemy->SetEnemyType(_enemy->EnemyType::MELEE);
	else if (hasLeader)
	{
		_enemy->SetEnemyType(_enemy->EnemyType::RANGED);
		rangedEnemies.push_back(_enemy);
	}
	else
	{
		_enemy->SetEnemyType(_enemy->EnemyType::RANGED_LEADER);
		rangedEnemies.push_back(_enemy);
		hasLeader = true;
	}
}
