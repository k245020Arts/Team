#include "BossSpecialAttack1.h"
#include "Boss.h"
#include "stateManager.h"
#include "Animator.h"

BossSpecialAttack1::BossSpecialAttack1()
{
	//animId = ID::B_S_ATTACK1;
	id = ID::B_S_ATTACK1;
	string = Function::GetClassNameC<BossSpecialAttack1>();
	a = 2;
	counter = 0;
}

BossSpecialAttack1::~BossSpecialAttack1()
{
}

void BossSpecialAttack1::Update()
{
	Boss* b = GetBase<Boss>();
	b->LookPlayer();
	
	if (a == 0)
	{
		velocity.y = 50;
		a = 1;
	}
	else if (a == 1)
	{
		//b->enemyBaseComponent.anim->Play(ID::B_S_ATTACK1);

		counter += 0.005f;

		velocity.y -= counter;
		if (b->obj->GetTransform()->position.y <= 450)
		{
			a == 2;
			velocity = 0;
			counter = 0;
			b->obj->GetTransform()->position.y = 450;
			b->enemyBaseComponent.state->ChangeState(ID::B_RUN);
		}
	}
	
	if (CheckHitKey(KEY_INPUT_Z))
		a = 0;

	b->obj->GetTransform()->position += velocity;
}

void BossSpecialAttack1::Draw()
{
}

void BossSpecialAttack1::Start()
{
	EnemyStateBase::Start();
}

void BossSpecialAttack1::Finish()
{
}
