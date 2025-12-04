#include "BossNormalAttack3.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"
#include "../../../../Camera/Camera.h"

BossNormalAttack3::BossNormalAttack3()
{
	//id = ID::B_N_ATTACK3;
	string = Function::GetClassNameC<BossNormalAttack3>();
	animId = ID::B_N_ATTACK3;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	damage.damagePattern = BossAttackBase::BACK;
}

BossNormalAttack3::~BossNormalAttack3()
{
}

void BossNormalAttack3::Update()
{
	Boss* b = GetBase<Boss>();
	const float MSPEED = 60.0f;//モーションの速度調整

	counter++;
	//モーションの速度減速
	if (counter < MSPEED && damage.motionSpeed >= 0)
		damage.motionSpeed -= damage.motionMaxSpeed / MSPEED;
	//モーションの速度加速
	else if (damage.motionSpeed <= damage.motionMaxSpeed)
		damage.motionSpeed += damage.motionMaxSpeed / MSPEED;
	//どこまでプレイヤーの方を見るか(今後回避行動取るまでに変更)
	if (counter <= 50)
		b->LookPlayer();

	b->enemyBaseComponent.anim->SetPlaySpeed(damage.motionSpeed);

	if (b->enemyBaseComponent.anim->IsFinish())
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_RUN_S);

	BossAttackCollsion();
	AttackSound();
	AttackFlash(ID::B_MODEL, b->BOSS_LEFT_HAND_FRAME, "E_AttackV");
	BossTrail(false);
}

void BossNormalAttack3::Draw()
{
}

void BossNormalAttack3::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();

	firstColl = true;
	counter = 0;
	damage.hitDamage = b->bs->GetStatus().normalAttack3;
	b->enemyBaseComponent.anim->AnimEventReset();

	damage.motionMaxSpeed = b->bs->GetStatus().motionSpeed;

	b->enemyBaseComponent.anim->SetPlaySpeed(damage.motionMaxSpeed);
	damage.motionSpeed = damage.motionMaxSpeed;
	b->enemyBaseComponent.camera->AttackEnemyFovChange(b->bossTransform);
}

void BossNormalAttack3::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	boss->enemyBaseComponent.anim->AnimEventReset();
	if (boss->maxAttack == 0)
		boss->enemyBaseComponent.anim->SetPlaySpeed(1.2f);
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
}
