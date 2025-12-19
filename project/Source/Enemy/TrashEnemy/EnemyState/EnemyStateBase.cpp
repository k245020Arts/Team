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
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed + 0.3f);
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
}

void EnemyStateBase::AttackCollsion()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	//敵の攻撃判定の生成クラス
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
	//敵の攻撃判定の生成クラス
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
	//敵の攻撃の音を発生
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
	//敵の剣回りを光らせていることへの設定
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		AttackBeforeFrash(_modelId, _modelFrame, _voice);
		//com.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(0, 0, 300) * MGetRotY(com.enemy->GetEnemyTransform()->rotation.y), 100.0f, 10.0f, 200.0f, 255.0f, 28, 1.0f);
	}
}

void EnemyStateBase::AttackBeforeFrash(ID::IDType _modelId, int _modelFrame, std::string _voice)
{
	EnemyBase* e = GetBase<EnemyBase>();
	if (sound) {

		MATRIX matrixWorld = MV1GetFrameLocalWorldMatrix(Load::GetHandle(_modelId), _modelFrame);
		// WORLD座標を取得（DXの関数。既にワールド座標の場合が多い）
		VECTOR3 frameWorldPos = MV1GetFramePosition(Load::GetHandle(_modelId), _modelFrame);

		
		MATRIX objWorldMat = obj->GetTransform()->WorldTransform().GetMatrix();
		MATRIX invObjWorldMat = MInverse(objWorldMat); // プロジェクトに合わせること
		VECTOR3 frashPosLocal = frameWorldPos * invObjWorldMat; // VECTOR3 * MATRIX の定義に依存
		//e->enemyBaseComponent.effect->CreateEffekseer(Transform(frashPos_local, VZero, VOne), obj, Effect_ID::ENEMY_FLASH, 1.0f);

		e->enemyBaseComponent.effect->CreateEffekseer(Transform(frashPosLocal, VZero, VOne), obj, Effect_ID::ENEMY_FLASH, 1.0f);
		e->enemyBaseComponent.sound->PlaySe(Sound_ID::ENEMY_ATTACK_BEFORE);
		e->enemyBaseComponent.sound->RandamSe(_voice, 3);
		//com.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(500, 500, 1000) * MGetRotY(com.enemy->GetEnemyTransform()->rotation.y), 100.0f, 10.0f, 200.0f, 255.0f, 28, 0.5f);
		sound = false;
		keepAnimSpeed = e->enemyBaseComponent.anim->GetPlaySpeed();
		e->enemyBaseComponent.anim->SetPlaySpeed(0.1f);
		animSlowCounter = 0.2f;
		
	}
}

void EnemyStateBase::Trail()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	float time = e->enemyBaseComponent.anim->EventStartTime(animId);
	//剣の軌跡を発生
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time + 5.0f >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		e->Trail();
	}
}

void EnemyStateBase::BossTrail(bool _right)
{
	Boss* e = GetBase<Boss>();
	float time = e->enemyBaseComponent.anim->EventStartTime(animId);
	//剣の軌跡を発生
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time + 5.0f >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		e->Drail(_right);
	}
}
