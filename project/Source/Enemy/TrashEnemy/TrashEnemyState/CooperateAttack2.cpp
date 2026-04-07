#include "CooperateAttack2.h"
#include "../TrashEnemy.h"
#include "T_EnemyStatus.h"

CooperateAttack2::CooperateAttack2()
{
	string = Function::GetClassNameC<CooperateAttack2>();
	//animId = ID::TE_ATTACK2;
	attackParam.animID = ID::TE_ATTACK2;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern = BossAttackBase::BACK;
}

CooperateAttack2::~CooperateAttack2()
{
}

void CooperateAttack2::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	const VECTOR3 pPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	e->LookTarget(pPos);

	const float ROTY = -e->enemyBaseComponent.playerObj->GetTransform()->rotation.y - 0.5f * DX_PI_F;

	e->GetEnemyObj()->GetTransform()->position.x += e->eStatus->GetStatus().runSpeed * cosf(ROTY);
	e->GetEnemyObj()->GetTransform()->position.z += e->eStatus->GetStatus().runSpeed * sinf(ROTY);
}

void CooperateAttack2::Start()
{
}

void CooperateAttack2::Finish()
{
}
