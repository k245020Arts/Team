#pragma once
#include "../EnemyBase.h"
#include "../enemyInformation.h"
#include "../../Component/Transform/transform.h"
#include "../../Common/ID/SoundID.h"


class StateManager;
class EnemyStateManager;

class TrashEnemy : public EnemyBase
{
public:
	TrashEnemy();
	~TrashEnemy();

	void Update()override;
	void Draw()override;

	void CreateTrashEnemy(VECTOR3 _pos);
private:

};