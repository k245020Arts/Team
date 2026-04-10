#include "BossNormalAttack1.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"

BossNormalAttack1::BossNormalAttack1()
{
	//id = ID::B_N_ATTACK1;
	string					= Function::GetClassNameC<BossNormalAttack1>();
	/*attackParam.animID					= ID::B_N_ATTACK1;
	attackParam.attackCollTransform				= Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern	= BossAttackBase::NO_BACK;*/
	
	LoadAttackParam();
	/*attackParam.rotateMove = true;
	attackParam.angleMoveAmout = 90.0f;

	attackParam.useTrail = true;
	attackParam.trailRightHand = true;*/
}

BossNormalAttack1::~BossNormalAttack1()
{
}

void BossNormalAttack1::Update()
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
	//	AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum,attackParam.voiceName);
	//	//attackParam.flash = true;
	//}
	//
	//BossTrail(true);
	
}

void BossNormalAttack1::Draw()
{
}

void BossNormalAttack1::Start()
{
	Boss* boss = GetBase<Boss>();
	//EnemyStateBase::Start();
	BossAttackBase::BossStart();
	firstColl = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	//attackParam.hitDamage = boss->bs->GetStatus().normalAttack1;
	boss->enemyBaseComponent.anim->SetFrame(5.0f);
	fallFrame = boss->bs->GetStatus().fallFrame;

}

void BossNormalAttack1::Finish()
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
