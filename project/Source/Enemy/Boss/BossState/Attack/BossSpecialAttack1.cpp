#include "BossSpecialAttack1.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../Component/Physics/Physics.h"
#include "../../../../Common/Effect/EffectManager.h"
#include "../../../../Component/Collider/DountCollider.h"
#include "../../../../Component/EnemyAttackObject/ShockWave/ShockWave.h"
#include "../../../../Common/Sound/SoundManager.h"
#include "../../../../Camera/Camera.h"

BossSpecialAttack1::BossSpecialAttack1()
{
	//animId = ID::B_S_ATTACK1;
	//id = ID::B_S_ATTACK1;

	string = Function::GetClassNameC<BossSpecialAttack1>();
	counter = 0;
	/*attackParam.animID = ID::B_S_ATTACK1;

	attackParam.attackCollTransform = Transform(VECTOR3(0, -50, 0), VZero, VECTOR3(500.0f, 0.0f, 0.0f));
	attackParam.damagePattern = BossAttackBase::BLOW_AWAY;
	attackParam.hitDamage = 50.0f;*/

	velocity = VZero;
	rotation = VZero;
	counter = 0.0f;
	copyPos = 0.0f;
	count = 0.0f;
	subSpeed = 0.0f;
	attackStart = 0.0f;
	firstCount = false;
	effect = false;
	LoadAttackParam();
	/*attackParam.playerAloowMove = true;
	attackParam.playerNearStop = true;
	attackParam.maxMoveSpeed = 30000.0f;
	attackParam.minMoveSpeed = 12000.0f;
	attackParam.playerBaseNear = 1000.0f;
	attackParam.lookPlayer = true;
	attackParam.lookNum = 1;
	attackParam.lookMaxCounter = 50.0f;
	attackParam.moveStartTime = 15.0f;
	attackParam.moveFinishTime = 45.0f;
	attackParam.addVelocity = true;

	attackParam.addGravity = -5.0f;
	attackParam.jump = true;
	attackParam.jumpGroundEffect = Effect_ID::BOSS_GROUND;
	attackParam.jumpSpeed = 3000.0f;
	attackParam.jumpStartTime = 15.0f;
	attackParam.groundEffectStartTime = 40.0f;
	attackParam.shockWave = true;
	attackParam.shockWaveSpeed = 50.0f;
	attackParam.startRange = 50.0f;
	attackParam.shockMoveEffect = Effect_ID::BOSS_WAVE;
	attackParam.groundShakeTime = 0.3f;
	attackParam.groundShakeCamera = 3.0f;*/
}

BossSpecialAttack1::~BossSpecialAttack1()
{
}

void BossSpecialAttack1::Update()
{
	Boss* b = GetBase<Boss>();
	BossAttackBase::Update();
	///*if (b->enemyBaseComponent.anim->GetCurrentFrame() <= attackStart) {
	//	return;
	//}
	//else {
	//	if (firstCount) {
	//		firstCount = false;
	//		b->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, 3000, 0), false);
	//		count = 1.0f;
	//		subSpeed = 0;
	//		VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;

	//		VECTOR3 sub = pos - b->GetBaseObject()->GetTransform()->position;
	//		b->enemyBaseComponent.physics->AddVelocity(sub, false);
	//		
	//	}
	//}*/

	//JumpEvent();
	//
	//if (b->enemyBaseComponent.physics->GetGround()) {
	//	/*if (b->enemyBaseComponent.anim->GetCurrentFrame() >= 40) {
	//		if (effect) {
	//			effect = false;
	//			BaseObject* obj1 = EffectManager::GetInstance()->CreateEffekseer(*b->GetBaseObject()->GetTransform(), b->GetBaseObject(), Effect_ID::BOSS_WAVE, 1.0f);
	//			BaseObject* obj2 = EffectManager::GetInstance()->CreateEffekseer(*b->GetBaseObject()->GetTransform(), b->GetBaseObject(), Effect_ID::BOSS_GROUND, 1.0f);
	//			ShockWave* w = obj1->Component()->AddComponent<ShockWave>();
	//			EffectManager::GetInstance()->ParentTransformRemove(obj1);
	//			EffectManager::GetInstance()->ParentTransformRemove(obj2);
	//			w->CreateWave(CollsionInformation::B_E_ATTACK, Transform(VZero, VZero, VOne), 50.0f, 50.0f);
	//			SoundManager::GetInstance()->PlaySe(Sound_ID::GROUND);
	//			b->enemyBaseComponent.camera->CameraPerspectiveShakeStart(3.0f, 0.4f);
	//		}
	//	}*/
	//	
	//	if (b->enemyBaseComponent.anim->IsFinish()) {
	//		b->BossAttackStateChange();
	//	}
	//}
	///*if (b->enemyBaseComponent.anim->GetCurrentFrame() <= b->enemyBaseComponent.anim->EventFinishTime(attackParam.animID)) {
	//	VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;
	//	VECTOR3 sub = pos - b->GetBaseObject()->GetTransform()->position;
	//	VECTOR3 ynotPos = sub * VECTOR3(1, 0, 1);	
	//	float size = ynotPos.Size();
	//	VECTOR3 move = ynotPos.Normalize() * size * 2.0f;
	//	move.y = b->enemyBaseComponent.physics->GetVelocity().y;
	//	b->enemyBaseComponent.physics->SetVelocity(move);
	//	b->LookPlayer();
	//}
	//else {
	//	b->enemyBaseComponent.physics->SetVelocity(VZero);
	//}*/
	//MoveEvent();
	//
	//BossAttackCollsion();
	//BossJustAvoidCollsion();
	//AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
	//damage.flash = true;
}

void BossSpecialAttack1::Draw()
{
}

void BossSpecialAttack1::Start()
{
	//EnemyStateBase::Start();
	Boss* b = GetBase<Boss>();
	BossAttackBase::BossStart();
	
	/*b->enemyBaseComponent.anim->AnimEventReset();
	firstColl = true;
	attackStart = 15.0f;
	firstCount = true;
	effect = true;
	b->threat = false;*/
}

void BossSpecialAttack1::Finish()
{
#ifdef DataSave
	DataSaveAll();
#endif // DataSave
	Boss* boss = GetBase<Boss>();
	BossAttackBase::BossFinish();
	/*boss->enemyBaseComponent.physics->SetGravity(VECTOR3(0, -1500, 0));
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);*/
	//b->threat = false;
}
