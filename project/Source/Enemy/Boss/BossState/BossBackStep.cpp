#include "BossBackStep.h"
#include "../../../Common/Easing.h"
#include "../../../State/StateManager.h"

BossBackStep::BossBackStep()
{
	animId = ID::B_BACKSTEP;
	//string = Function::GetClassNameC<BossCoolTime>();

	speed = 0.0f;
	time = 0.0f;
	counter = 0;
}

BossBackStep::~BossBackStep()
{
}

void BossBackStep::Update()
{
	//å„Ç≈èëÇ´íºÇ∑
	Boss* b = GetBase<Boss>();
	b->LookPlayer();
	speed = Easing::EaseIn(0.0f, MaxSpeed, MaxSpeed / 60);

	const float ROTY = -b->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F;
	
	time += Time::DeltaTimeRate();

	if (vec.Size() <= 2200.0f && counter == 1|| vec.Size() <= 4400 && counter == 2)
	{
		b->GetEnemyObj()->GetTransform()->position.x -= speed * cosf(ROTY);
		b->GetEnemyObj()->GetTransform()->position.z -= speed * sinf(ROTY);
	}
	else if (counter < 2)
	{
		time = 0.0f;
		counter++;
	}
	
	if(time>=0.55f||time >= 7.0f)
		b->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
}

void BossBackStep::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	speed = 0.0f;
	time = 0.0f;
	counter = 0;

	vec = b->enemyBaseComponent.playerObj->GetTransform()->position - b->GetEnemyObj()->GetTransform()->position;
}

void BossBackStep::Finish()
{
}
