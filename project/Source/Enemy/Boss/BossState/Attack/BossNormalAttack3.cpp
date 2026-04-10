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
	/*attackParam.animID = ID::B_N_ATTACK3;
	attackParam.attackCollTransform				= Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern	= BossAttackBase::BACK;*/
	counter					= 0;
	LoadAttackParam();
	attackParam.lookPlayer = true;
	attackParam.lookNum = 1;
	attackParam.lookMaxCounter = 40.0f;
	attackParam.attackCameraBossLook = true;
	attackParam.cameraChangeSpeed = 1000.0f;

	attackParam.useTrail = true;
	attackParam.trailRightHand = false;
}

BossNormalAttack3::~BossNormalAttack3()
{
}

void BossNormalAttack3::Update()
{
	Boss* b = GetBase<Boss>();
	//const float MSPEED = 60.0f;//モーションの速度調整

	BossAttackBase::Update();

	////どこまでプレイヤーの方を見るか(今後回避行動取るまでに変更)
	//LookEvent();

	////b->enemyBaseComponent.anim->SetPlaySpeed(damage.motionSpeed);

	//if (b->enemyBaseComponent.anim->IsFinish())
	//	b->BossAttackStateChange();

	//BossAttackCollsion();
	//BossJustAvoidCollsion();
	//AttackSound();
	//if (b->maxAttack <= 0) {
	//	AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
	//	//attackParam.damage = true;
	//}

	//BossTrail(false);
}

void BossNormalAttack3::Draw()
{
}

void BossNormalAttack3::Start()
{
	Boss* b = GetBase<Boss>();
	//EnemyStateBase::Start();
	BossAttackBase::BossStart();

	//firstColl				= true;
	//counter					= 0;
	////attackParam.hitDamage		= b->bs->GetStatus().normalAttack3;
	//b->enemyBaseComponent.anim->AnimEventReset();

	////damage.motionMaxSpeed	=;

	////b->enemyBaseComponent.anim->SetPlaySpeed(damage.motionMaxSpeed);
	////damage.motionSpeed		= b->bs->GetStatus().motionSpeed;
	////b->enemyBaseComponent.camera->AttackEnemyFovChange(b->bossTransform,1000.0f);

	//b->threat				= false;
}

void BossNormalAttack3::Finish()
{
#ifdef DataSave
	DataSaveAll();
#endif // DataSave
	Boss* boss = GetBase<Boss>();
	/*boss->DeleteCollision(&boss->attackColl);*/
	BossAttackBase::BossFinish();
	/*boss->enemyBaseComponent.anim->AnimEventReset();
	if (boss->maxAttack == 0)
		boss->enemyBaseComponent.anim->SetPlaySpeed(1.2f);
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);*/
	
	//boss->threat = true;
}
