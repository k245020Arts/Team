#pragma once
#include "../EnemyBase.h"
#include "../../Component/Object/Object3D.h"
#include "../EnemyInformation.h"

class StateManager;
class EnemyStateManager;

class TrashEnemy : public EnemyBase
{
public:
	friend class T_EnemyIdol;

	TrashEnemy();
	~TrashEnemy();

	void Update()override;
	void Draw()override;

	void Start();

	void CreateTrashEnemy(VECTOR3 _pos);
	VECTOR3 GetPos() { return obj->GetTransform()->position; }

private:
};