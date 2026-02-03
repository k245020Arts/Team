#include "T_EnemyAttack.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"
#include "../../../Player/Player.h"
#include "../../../Common/Easing.h"

T_EnemyAttack::T_EnemyAttack()
{
	string = Function::GetClassNameC<T_EnemyAttack>();
	animId = ID::TE_ATTACK;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));

	damage.damagePattern = BossAttackBase::NO_BACK;

	mSpeed = 0;
	mMaxFrame = 0;

	isDecel = true;
}

T_EnemyAttack::~T_EnemyAttack()
{
}

void T_EnemyAttack::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->LookTarget();
	
	if (e->isCooperateAtk)
		return;

	counter++;
	if (counter >= 5 && counter <= 10)
	{
		e->GetEnemyObj()->GetTransform()->position.x += 40 * cosf(-e->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F);
		e->GetEnemyObj()->GetTransform()->position.z += 40 * sinf(-e->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F);
	}

	const float M_FRAME_SPEED = mMaxFrame / 2 / 2;

	if (mSpeed > 0 && isDecel)
		mSpeed = Easing::EaseOut(mMaxFrame, 0.0f, M_FRAME_SPEED);
	else
	{
		isDecel = false;
		mSpeed = Easing::EaseIn(mSpeed, mMaxFrame, M_FRAME_SPEED);
	}

	e->enemyBaseComponent.anim->SetPlaySpeed(mSpeed);
	
	/*if (e->enemyBaseComponent.anim->IsFinish())
		e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);*/

	AttackInformation(e);
}

void T_EnemyAttack::Draw()
{
}

void T_EnemyAttack::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	firstColl = true;
	e->enemyBaseComponent.anim->SetFrame(5.0f);
	damage.hitDamage = e->eStatus->GetStatus().normalAttack1;
	counter = 0;
	e->isAttack = false;
	mMaxFrame = e->enemyBaseComponent.anim->GetMaxFrame();
	isDecel = true;
}

void T_EnemyAttack::Finish()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	e->isAttack = false;
	e->DeleteCollision();
}

void T_EnemyAttack::AttackInformation(TrashEnemy* _e)
{
	if (_e->enemyBaseComponent.anim->IsFinish())
		_e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);

	AttackCollsion();
	AttackSound();
	AttackFlash(ID::E_MODEL, 35, "E_AttackV");
	_e->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	Trail();
}
