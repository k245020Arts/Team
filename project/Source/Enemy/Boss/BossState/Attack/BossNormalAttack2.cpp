#include "BossNormalAttack2.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"

BossNormalAttack2::BossNormalAttack2()
{
	//id = ID::B_N_ATTACK2;
	string					= Function::GetClassNameC<BossNormalAttack2>();
	/*attackParam.animID = ID::B_N_ATTACK2;
	attackParam.attackCollTransform				= Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern	= BossAttackBase::NO_BACK;*/
	LoadAttackParam();
	/*attackParam.rotateMove = true;
	attackParam.angleMoveAmout = -90.0f;

	attackParam.useTrail = true;
	attackParam.trailRightHand = false;*/
}

BossNormalAttack2::~BossNormalAttack2()
{
}

void BossNormalAttack2::Update()
{
	BossAttackBase::Update();
	//Boss* boss = GetBase<Boss>();
	//if (boss->enemyBaseComponent.anim->IsFinish())
	//{
	//	boss->BossAttackStateChange();
	//}
	//BossAttackCollsion();
	//BossJustAvoidCollsion();
	//RotateEvent();
	//AttackSound();
	//if (boss->maxAttack <= 0) {
	//	//ŽO’iUŒ‚‚Ìˆê”ÔÅŒã‚ÌŽž‚¾‚¯Œõ‚éB
	//	AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
	//	//attackParam.flash = true;
	//}

	//BossTrail(false);
}

void BossNormalAttack2::Draw()
{
}

void BossNormalAttack2::Start()
{
	Boss* boss			= GetBase<Boss>();
	//EnemyStateBase::Start();
	BossAttackBase::BossStart();
	//firstColl			= true;
	////attackParam.hitDamage	= boss->bs->GetStatus().normalAttack1;

	//fallFrame			= boss->bs->GetStatus().fallFrame;
	//boss->enemyBaseComponent.anim->SetFrame(5.0f);
	//boss->threat		= false;
}

void BossNormalAttack2::Finish()
{
#ifdef DataSave
	DataSaveAll();
#endif // DataSave
	Boss* boss = GetBase<Boss>();
	/*boss->DeleteCollision(&boss->attackColl);*/
	BossAttackBase::BossFinish();
	/*boss->enemyBaseComponent.anim->AnimEventReset();
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);*/
	//boss->threat = true;
}
