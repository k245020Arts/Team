#include "EnemyStateBase.h"
#include "../../../Component/Animator/Animator.h"
#include "../Enemy.h"
#include "../../../Component/Transform/transform.h"
#include "../../../Component/Collider/sphereCollider.h"
#include "../../EnemyBase.h"
#include "../../Boss/Boss.h"
#include "../../../Common/LoadManager.h"
#include "EnemyStateManager.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Weapon/WeaponManager.h"
#include "../../../Camera/Camera.h"
#include "../TrashEnemy.h"

#define PATTERN1

EnemyStateBase::EnemyStateBase()
{
	firstColl = false;
	animId = ID::E_GETUP;
	attackTime = 0.0f;
	keepAnimSpeed = 0.0f;



	collTrans = Transform();
	sound = false;;

	fallFrame = 0.0f;
	animSlowCounter = 0.0f;
	slowMode = false;
}

EnemyStateBase::~EnemyStateBase()
{
}

void EnemyStateBase::Update()
{
	EnemyBase* e = GetBase<EnemyBase>();
	if (animSlowCounter > 0.0f) {
		animSlowCounter -= Time::DeltaTimeRate();
		if (animSlowCounter < 0.0f) {
#ifdef PATTERN1
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed);
			slowMode = true;
#else
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed - 0.7f);
			/*if (!e->enemyBaseComponent.anim->AnimEventCan()) {
				e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed + 0.3f);
			}*/
#endif
		}
	}
	if (slowMode) {
		if (e->enemyBaseComponent.anim->EventFinishTime(animId) - 5.0f <= e->enemyBaseComponent.anim->GetCurrentFrame()) {
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed + 0.3f);
			slowMode = false;
		}
	}
}

void EnemyStateBase::Start()
{
	EnemyBase* e = GetBase<EnemyBase>();
	e->enemyBaseComponent.anim->Play(animId);
	attackTime = e->enemyBaseComponent.anim->EventFinishTime(animId) - e->enemyBaseComponent.anim->EventStartTime(animId);
	sound = true;
	e->enemyBaseComponent.anim->AnimEventReset();
	slowMode = false;
}

void EnemyStateBase::AttackCollsion()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	//“G‚ÌUŒ‚”»’è‚Ì¶¬ƒNƒ‰ƒX
	if (e->enemyBaseComponent.anim->AnimEventCan()) {
		if (firstColl) {
			firstColl = false;
			e->CollsionStart<SphereCollider>(CollsionInformation::SPHERE,collTrans);
			e->SetShape(CollsionInformation::SPHERE);
		}
	}
}

void EnemyStateBase::BossAttackCollsion()
{
	Boss* b = GetBase<Boss>();
	//“G‚ÌUŒ‚”»’è‚Ì¶¬ƒNƒ‰ƒX
	if (b->enemyBaseComponent.anim->AnimEventCan()) {
		if (firstColl) {
			firstColl = false;
			b->CollsionStart<SphereCollider>(CollsionInformation::SPHERE, collTrans);
			b->SetShape(CollsionInformation::SPHERE);
		}
	}
}

void EnemyStateBase::AttackSound()
{
	EnemyBase* e = GetBase<EnemyBase>();
	float time = e->enemyBaseComponent.anim->EventStartTime(animId);
	//“G‚ÌUŒ‚‚Ì‰¹‚ğ”­¶
	if (time - 1.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		if (!e->enemyBaseComponent.sound->CheckSe(Sound_ID::ENEMY_SWORD_WIND1) && !e->enemyBaseComponent.sound->CheckSe(Sound_ID::ENEMY_SWORD_WIND2)) {
			e->enemyBaseComponent.sound->RandamSe("EnemySword", 2);
		}
	}
}

void EnemyStateBase::AttackFlash(ID::IDType _modelId, int _modelFrame, std::string _voice)
{
	EnemyBase* e = GetBase<EnemyBase>();
	float time = e->enemyBaseComponent.anim->EventStartTime(animId);
	//“G‚ÌŒ•‰ñ‚è‚ğŒõ‚ç‚¹‚Ä‚¢‚é‚±‚Æ‚Ö‚Ìİ’è
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		if (_modelId == ID::E_MODEL) {
			int a = 0;
		}
		AttackBeforeFrash(_modelId, _modelFrame, _voice);
		//com.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(0, 0, 300) * MGetRotY(com.enemy->GetEnemyTransform()->rotation.y), 100.0f, 10.0f, 200.0f, 255.0f, 28, 1.0f);
	}
}

void EnemyStateBase::AttackBeforeFrash(ID::IDType _modelId, int _modelFrame, std::string _voice)
{
	EnemyBase* e = GetBase<EnemyBase>();
	if (sound) {

		MATRIX matrixWorld = MV1GetFrameLocalWorldMatrix(Load::GetHandle(_modelId), _modelFrame);
		// WORLDÀ•W‚ğæ“¾
		VECTOR3 frameWorldPos = MV1GetFramePosition(Load::GetHandle(_modelId), _modelFrame);

		
		MATRIX objWorldMat = obj->GetTransform()->WorldTransform().GetMatrix();
		MATRIX invObjWorldMat = MInverse(objWorldMat); 
		VECTOR3 frashPosLocal = frameWorldPos * invObjWorldMat;
		//e->enemyBaseComponent.effect->CreateEffekseer(Transform(frashPos_local, VZero, VOne), obj, Effect_ID::ENEMY_FLASH, 1.0f);
		if (_modelId == ID::E_MODEL) {
			e->enemyBaseComponent.effect->CreateEffekseer(Transform(frameWorldPos + obj->GetTransform()->position, VZero, VOne), nullptr, Effect_ID::ENEMY_FLASH, 1.0f);
		}
		else {
			e->enemyBaseComponent.effect->CreateEffekseer(Transform(frashPosLocal, VZero, VOne), obj, Effect_ID::ENEMY_FLASH, 1.0f);
		}
		
		e->enemyBaseComponent.sound->PlaySe(Sound_ID::ENEMY_ATTACK_BEFORE);
		e->enemyBaseComponent.sound->RandamSe(_voice, 3);
		//com.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(500, 500, 1000) * MGetRotY(com.enemy->GetEnemyTransform()->rotation.y), 100.0f, 10.0f, 200.0f, 255.0f, 28, 0.5f);
		sound = false;
		keepAnimSpeed = e->enemyBaseComponent.anim->GetPlaySpeed();
		e->enemyBaseComponent.anim->SetPlaySpeed(0.1f);
		animSlowCounter = 0.3f;
		
	}
}

void EnemyStateBase::Trail()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	float time = e->enemyBaseComponent.anim->EventStartTime(animId);
	//Œ•‚Ì‹OÕ‚ğ”­¶
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time + 5.0f >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		e->Trail();
	}
}

void EnemyStateBase::BossTrail(bool _right)
{
	Boss* e = GetBase<Boss>();
	float time = e->enemyBaseComponent.anim->EventStartTime(animId);
	//Œ•‚Ì‹OÕ‚ğ”­¶
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time + 5.0f >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		e->Drail(_right);
	}
}
