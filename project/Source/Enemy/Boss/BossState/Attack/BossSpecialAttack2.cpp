#include "BossSpecialAttack2.h"
#include "../../Boss.h"
#include "../../../../State/StateManager.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../Component/Collider/SphereCollider.h"
#include "../../../../Component/Physics/Physics.h"
#include "../../../../Camera/Camera.h"
#include "../../../../Common/Sound/SoundManager.h"

BossSpecialAttack2::BossSpecialAttack2()
{
	//id = ID::B_S_ATTACK2;
	string = Function::GetClassNameC<BossSpecialAttack2>();
	//attackParam.animID = ID::B_S_ATTACK2_BEFORE;
	//attackParam.attackCollTransform = Transform(VECTOR3(0, 0, -50), VZero, VECTOR3(500.0f, 0.0f, 0.0f));
	////damage.damagePattern = BossAttackBase::BLOW_AWAY;
	//attackParam.hitDamage = 50.0f;

	attackCount = 0.0f;
	rotation = VZero;
	distance = 0.0f;
	look = false;

	turningTime = 0.0f;

	
	LoadAttackParam();
	attackParam.playerAloowMove = true;
	attackParam.playerNearAloowStop = true;
	attackParam.maxMoveSpeed = 15000.0f;
	attackParam.minMoveSpeed = 15000.0f;
	attackParam.playerBaseNear = 2000.0f;
	attackParam.baseSpeed = 15000.0f;
	attackParam.lookPlayer = true;
	attackParam.lookNum = 1;
	attackParam.lookMaxCounter = 200.0f;
	attackParam.moveStartTime = 00.0f;
	attackParam.moveFinishTime = 500.0f;
	attackParam.addVelocity = true;

	attackParam.rushMove = true;
	attackParam.rushAfterAnimID = ID::B_S_ATTACK2_STOP;
	attackParam.attackBeforeAnimID = ID::B_S_ATTACK2_BEFORE;
	attackParam.rushAfterSpeed = 5000.0f;
	attackParam.rushColl = true;
	attackParam.addRushCollScale = 50.0f;
	attackParam.rushSoundRightFoot = 16.0f;
	attackParam.rushSoundLeftFoot = 6.0f;
	attackParam.rushTime = 1.0f;

	attackParam.attackCameraBossLook = true;
	attackParam.cameraChangeSpeed = 1000.0f;

}

BossSpecialAttack2::~BossSpecialAttack2()
{
}

void BossSpecialAttack2::Update()
{
	Boss* b = GetBase<Boss>();
	BossAttackBase::Update();
	//RushEvent();
	////b->enemyBaseComponent.physics->AddVelocity(rotation * 10000.0f, true);
	//BossAttackCollsion();
	//BossJustAvoidCollsion();
	//MoveEvent();
	/*VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;
	VECTOR3 angle = pos - b->GetBaseObject()->GetTransform()->position;
	distance = angle.Size();
	if (b->maxAttack <= 0) {
		b->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, 0, -15000) * b->GetEnemyObj()->GetTransform()->GetRotationMatrix(), true);
	}
	else {
		b->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, 0, -15000) * b->GetEnemyObj()->GetTransform()->GetRotationMatrix(), true);
	}
	if (!look) {
		return;
	}
	if (distance > 2000.0f) {
		b->LookPlayer(0.2f);
	}
	else {
		look = false;
	}*/

}

void BossSpecialAttack2::Draw()
{

}

void BossSpecialAttack2::Start()
{
	Boss* b = GetBase<Boss>();
	//EnemyStateBase::Start();
	BossAttackBase::BossStart();
	
	
	

}

void BossSpecialAttack2::Finish()
{
#ifdef DataSave
	DataSaveAll();
#endif // DataSave
	//EnemyStateBase::Finish();
	Boss* b = GetBase<Boss>();
	BossAttackBase::BossFinish();
	//if (b->maxAttack > 0) {
	//	b->LookPlayer(1.0f);
	//	//‘Oi
	//	b->enemyBaseComponent.physics->SetVelocity(VECTOR3(0, 0, -1000) * b->GetEnemyObj()->GetTransform()->GetRotationMatrix());
	//	
	//}
	//b->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	//
	//b->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);
}
