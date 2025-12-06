#include "Standby.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

Standby::Standby()
{
}

Standby::~Standby()
{
}

void Standby::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->LookTarget();

}

void Standby::Draw()
{
}

void Standby::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->attackId = StateID::COOPERATEATTACK1;
	e->targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	EnemyStateBase::Start();
}

void Standby::Finish()
{
}
