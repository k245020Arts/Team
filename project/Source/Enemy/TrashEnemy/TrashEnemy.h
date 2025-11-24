#pragma once
#include "../EnemyBase.h"
#include "../../Component/Object/Object3D.h"
#include "../EnemyInformation.h"

class StateManager;
class EnemyStateManager;
class CharaWeapon;

class TrashEnemy : public EnemyBase
{
public:
	friend class T_EnemyIdol;
	friend class T_EnemyRun;
	friend class T_EnemyAttack;

	TrashEnemy();
	~TrashEnemy();

	void Update()override;
	void Draw()override;

	void Start(Object3D* _obj);

	void CreateTrashEnemy(VECTOR3 _pos);
	VECTOR3 GetPos() { return obj->GetTransform()->position; }

	void Trail();

private:
	CharaWeapon* chara;
	void LookPlayer();
};