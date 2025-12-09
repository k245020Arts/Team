#include "Standby.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

Standby::Standby()
{
	counter = 0;
}

Standby::~Standby()
{
}

void Standby::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->LookTarget();
	
	counter++;

	if (counter == 100)
		e->isStandby = true;
	else
		e->isStandby = false;
}

void Standby::Draw()
{
}

void Standby::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->attackId = StateID::COOPERATEATTACK1;
	e->targetPos = e->enemyBaseComponent.playerObj->GetTransform()->position;
	e->speed = 10;
	//counter = 0;
	EnemyStateBase::Start();
}

void Standby::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	counter = 0;
	e->isStandby = false;
}
