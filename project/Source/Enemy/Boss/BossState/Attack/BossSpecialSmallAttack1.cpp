#include "BossSpecialSmallAttack1.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../Component/Physics/Physics.h"
#include "../../../../Common/Effect/EffectManager.h"
#include "../../../../Component/Collider/DountCollider.h"
#include "../../../../Component/EnemyAttackObject/ShockWave/ShockWave.h"
#include "../../../../Common/Sound/SoundManager.h"
#include "../../../../Camera/Camera.h"

BossSpecialSmallAttack1::BossSpecialSmallAttack1()
{
	//id = ID::B_S_ATTACK1_SMALL;
	string					= Function::GetClassNameC<BossSpecialSmallAttack1>();
	/*attackParam.animID = ID::B_S_ATTACK1_SMALL;

	attackParam.attackCollTransform = Transform(VECTOR3(0, -50, 0), VZero, VECTOR3(500.0f, 0.0f, 0.0f));
	attackParam.damagePattern	= BossAttackBase::BACK;
	attackParam.hitDamage		= 50.0f;*/

	subSpeed				= 0.0f;
	attackStart				= 0.0f;
	firstCount				= false;
	effect					= false;
	LoadAttackParam();

	/*attackParam.playerAloowMove = true;
	attackParam.playerNearStop = true;
	attackParam.maxMoveSpeed = 30000.0f;
	attackParam.minMoveSpeed = 12000.0f;
	attackParam.playerBaseNear = 500.0f;
	attackParam.lookPlayer = true;
	attackParam.lookNum = 1;
	attackParam.lookMaxCounter = 58.0f;
	attackParam.moveStartTime = 35.0f;
	attackParam.moveFinishTime = 58.0f;
	attackParam.addVelocity = true;

	attackParam.addGravity = -10.0f;
	attackParam.jump = true;
	attackParam.jumpGroundEffect = Effect_ID::BOSS_GROUND;
	attackParam.groundEffectStartTime = 50.0f;
	attackParam.jumpSpeed = 3000.0f;
	attackParam.jumpStartTime = 30.0f;
	attackParam.shockWave = true;
	attackParam.shockWaveSpeed = 50.0f;
	attackParam.startRange = 50.0f;
	attackParam.shockMoveEffect = Effect_ID::BOSS_WAVE;
	attackParam.groundShakeTime = 0.3f;
	attackParam.groundShakeCamera = 3.0f;*/
}

BossSpecialSmallAttack1::~BossSpecialSmallAttack1()
{
}

void BossSpecialSmallAttack1::Update()
{
	Boss* b = GetBase<Boss>();

	BossAttackBase::Update();

	//if (b->enemyBaseComponent.anim->GetCurrentFrame() <= attackStart) {
	//	return;
	//}
	//else {
	//	if (firstCount) {
	//		//飛ぶ前のしゃがみからとびに移る。
	//		firstCount = false;
	//		b->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, 3000, 0), false);
	//		subSpeed = 0;
	//		VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;

	//		VECTOR3 sub = pos - b->GetBaseObject()->GetTransform()->position;
	//		b->enemyBaseComponent.physics->AddVelocity(sub, false);
	//	}
	//}

	//b->enemyBaseComponent.physics->AddGravity(VECTOR3(0, -subSpeed, 0));
	//subSpeed += 100.0f;

	//JumpEvent();

	//if (b->enemyBaseComponent.physics->GetGround()) {
	//	//if (b->enemyBaseComponent.anim->GetCurrentFrame() >= 40) {
	//	//	if (effect) {
	//	//		effect = false;
	//	//		//エフェクトと衝撃波の生成
	//	//		BaseObject* obj1	= EffectManager::GetInstance()->CreateEffekseer(*b->GetBaseObject()->GetTransform(), b->GetBaseObject(), Effect_ID::BOSS_WAVE, 1.0f);
	//	//		BaseObject* obj2	= EffectManager::GetInstance()->CreateEffekseer(*b->GetBaseObject()->GetTransform(), b->GetBaseObject(), Effect_ID::BOSS_GROUND, 1.0f);
	//	//		ShockWave* w		= obj1->Component()->AddComponent<ShockWave>();
	//	//		EffectManager::GetInstance()->ParentTransformRemove(obj1);
	//	//		EffectManager::GetInstance()->ParentTransformRemove(obj2);

	//	//		w->CreateWave(CollsionInformation::B_E_ATTACK, Transform(VZero, VZero, VOne), 50.0f, 50.0f);
	//	//		SoundManager::GetInstance()->PlaySe(Sound_ID::GROUND);
	//	//		b->enemyBaseComponent.camera->CameraPerspectiveShakeStart(1.0f, 0.4f);
	//	//	}
	//	//}

	//	if (b->enemyBaseComponent.anim->IsFinish()) {
	//		b->BossAttackStateChange();
	//	}
	//}
	//MoveEvent();
	///*if (b->enemyBaseComponent.anim->GetCurrentFrame() <= b->enemyBaseComponent.anim->EventFinishTime(attackParam.animID)) {

	//	VECTOR3 pos		= b->enemyBaseComponent.playerObj->GetTransform()->position;
	//	VECTOR3 sub		= pos - b->GetBaseObject()->GetTransform()->position;
	//	VECTOR3 ynotPos = sub * VECTOR3(1, 0, 1);
	//	float size		= ynotPos.Size();
	//	VECTOR3 move	= ynotPos.Normalize() * size;
	//	move.y			= b->enemyBaseComponent.physics->GetVelocity().y;
	//	b->enemyBaseComponent.physics->SetVelocity(move);

	//	b->LookPlayer();
	//}
	//else {
	//	b->enemyBaseComponent.physics->SetVelocity(VZero);
	//}*/


	//BossAttackCollsion();
	//BossJustAvoidCollsion();
}

void BossSpecialSmallAttack1::Draw()
{
}

void BossSpecialSmallAttack1::Start()
{
	BossAttackBase::BossStart();
	Boss* b = GetBase<Boss>();

	//b->enemyBaseComponent.anim->AnimEventReset();
	//firstColl	= true;
	//attackStart = 30.0f;
	//firstCount	= true;
	//effect		= true;
	////ポンポン攻撃を繰り出したいので、最後の隙をなくすために最終フレームを変更
	//b->enemyBaseComponent.anim->SetMaxFrame(attackParam.animID, 60.0f);

	//if (b->comboFirstAttack)
	//	b->enemyBaseComponent.anim->SetFrame(0.0f);
	//else
	//	b->enemyBaseComponent.anim->SetFrame(20.0f);
	
}

void BossSpecialSmallAttack1::Finish()
{
#ifdef DataSave
	DataSaveAll();
#endif // DataSave
	BossAttackBase::BossFinish();
	/*Boss* b = GetBase<Boss>();
	b->enemyBaseComponent.physics->SetGravity();
	b->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	b->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);*/
}
