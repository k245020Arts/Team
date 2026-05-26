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
	attackParam.animID = ID::TE_ATTACK;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));

	attackParam.damagePattern = BossAttackBase::NO_BACK;

	mSpeed = 0;
	mMaxFrame = 0;

	isDecel = true;
	attackParam.useFlash = true;
	attackParam.attackFlashStartTime = 0.7f;
	attackParam.slowAmout = 0.1f;
	attackParam.slowTime = 0.3f;
	attackParam.speedUpMotionSpeed = 0.3f;
}

T_EnemyAttack::~T_EnemyAttack()
{
}

void T_EnemyAttack::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	enemy->LookTarget(enemy->enemyBaseComponent.playerObj->GetTransform()->position);
	
	if (enemy->isCooperateAtk)
		return;

	counter+=Time::DeltaTimeRate();

	if (counter <= 0.3f)
	{
		const float Speed = 30.0f;
		enemy->GetEnemyObj()->GetTransform()->position.x += Speed * cosf(-enemy->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F);
		enemy->GetEnemyObj()->GetTransform()->position.z += Speed * sinf(-enemy->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F);
	}

	const float M_FRAME_SPEED = mMaxFrame * 0.5f;

	if (mSpeed > 0 && isDecel)
		mSpeed = Easing::EaseOut(mMaxFrame, 0.0f, M_FRAME_SPEED);

	enemy->enemyBaseComponent.anim->SetPlaySpeed(mSpeed);
	
	AttackInformation(enemy);
}

void T_EnemyAttack::Draw()
{
}

void T_EnemyAttack::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	firstColl = true;
	attackParam.hitDamage = enemy->eStatus->GetStatus().normalAttack1;
	counter = 0;
	mMaxFrame = enemy->enemyBaseComponent.anim->GetMaxFrame();
	isDecel = true;

	EnemyStateBase::Start();
}

void T_EnemyAttack::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	enemy->isAttack = false;
	enemy->DeleteCollision(&enemy->attackColl);
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
	EnemyJustAvoidCollsion();
}
