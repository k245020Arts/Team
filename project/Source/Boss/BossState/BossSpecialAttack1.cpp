#include "BossSpecialAttack1.h"
#include "../Boss.h"
#include "../../State/StateManager.h"
#include "../../Component/Animator/Animator.h"
#include "../../Component/Physics/Physics.h"

BossSpecialAttack1::BossSpecialAttack1()
{
	//animId = ID::B_S_ATTACK1;
	id = ID::B_S_ATTACK1;
	string = Function::GetClassNameC<BossSpecialAttack1>();
	a = 0;
	counter = 0;
}

BossSpecialAttack1::~BossSpecialAttack1()
{
}

void BossSpecialAttack1::Update()
{
	Boss* b = GetBase<Boss>();

	b->enemyBaseComponent.physics->AddGravity(VECTOR3(0, -100, 0));
	
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
		else
		{
			b->LookPlayer();
			rotation = b->obj->GetTransform()->rotation;

			float c = 5.0f;
			velocity.x = c * cosf(-rotation.y - 0.5f * DX_PI_F);
			velocity.z = c * sinf(-rotation.y - 0.5f * DX_PI_F);
		}
	}
	
	//b->obj->GetTransform()->position += velocity;
}

void BossSpecialAttack1::Draw()
{
}

void BossSpecialAttack1::Start()
{
	EnemyStateBase::Start();
	Boss* b = GetBase<Boss>();
	b->enemyBaseComponent.physics->AddVelocity(VECTOR3(0,1500,0),false);
}

void BossSpecialAttack1::Finish()
{
	Boss* b = GetBase<Boss>();
	b->enemyBaseComponent.physics->SetGravity(VECTOR3(0, -1500, 0));
}
