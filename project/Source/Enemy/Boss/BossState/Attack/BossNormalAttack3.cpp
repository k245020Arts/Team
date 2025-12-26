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
	string					= Function::GetClassNameC<BossNormalAttack3>();
	animId					= ID::B_N_ATTACK3;
	collTrans				= Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	damage.damagePattern	= BossAttackBase::BACK;
	counter					= 0;
}

BossNormalAttack3::~BossNormalAttack3()
{
}

void BossNormalAttack3::Update()
{
	Boss* b = GetBase<Boss>();
	const float MSPEED = 60.0f;//モーションの速度調整

	EnemyStateBase::Update();

	//どこまでプレイヤーの方を見るか(今後回避行動取るまでに変更)
	if (counter <= 50)
		b->LookPlayer();

	//b->enemyBaseComponent.anim->SetPlaySpeed(damage.motionSpeed);

	if (b->enemyBaseComponent.anim->IsFinish())
		b->BossAttackStateChange();

	BossAttackCollsion();
	AttackSound();
	if (b->maxAttack <= 0) {
		AttackFlash(ID::B_MODEL, b->BOSS_RIGHT_HAND_FRAME, "E_AttackV");
		damage.flash = true;
	}

	BossTrail(false);
}

void BossNormalAttack3::Draw()
{
}

void BossNormalAttack3::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	BossAttackBase::BossStart();

	firstColl				= true;
	counter					= 0;
	damage.hitDamage		= b->bs->GetStatus().normalAttack3;
	b->enemyBaseComponent.anim->AnimEventReset();

	damage.motionMaxSpeed	= b->bs->GetStatus().motionSpeed;

	//b->enemyBaseComponent.anim->SetPlaySpeed(damage.motionMaxSpeed);
	damage.motionSpeed		= damage.motionMaxSpeed;
	b->enemyBaseComponent.camera->AttackEnemyFovChange(b->bossTransform,1000.0f);

	b->threat				= false;
}

void BossNormalAttack3::Finish()
{
	Boss* boss = GetBase<Boss>();
	boss->DeleteCollision();
	BossAttackBase::BossFinish();
	boss->enemyBaseComponent.anim->AnimEventReset();
	if (boss->maxAttack == 0)
		boss->enemyBaseComponent.anim->SetPlaySpeed(1.2f);
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	//boss->threat = true;
}
