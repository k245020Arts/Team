#include "BossAttackBase.h"
#include "../../Boss.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../CharaBase/CharaBase.h"
#include "../../../../Component/Animator/Animator.h"
//#include "../Enemy.h"
#include "../../../../Component/Transform/transform.h"
#include "../../../../Component/Collider/sphereCollider.h"
#include "../../../EnemyBase.h"
#include "../../../Boss/Boss.h"
#include "../../../../Common/ResourceLoader.h"
#include "../../../TrashEnemy/EnemyState/EnemyStateManager.h"
#include "../../../../Common/Effect/EffectManager.h"
#include "../../../../Common/Sound/SoundManager.h"
#include "../../../../Weapon/WeaponManager.h"
#include "../../../../Camera/Camera.h"
#include "../../../TrashEnemy/TrashEnemy.h"
#include "../../../../Common/Debug/DebugLogText.h"

#define PATTERN2

BossAttackBase::BossAttackBase()
{
	attackParam = BossAttackParam();
	
}

BossAttackBase::~BossAttackBase()
{
	

}

void BossAttackBase::Update()
{
	EnemyBase* e = GetBase<EnemyBase>();
	if (animSlowCounter > 0.0f) {
		animSlowCounter -= Time::DeltaTimeRate();
		if (animSlowCounter < 0.0f) {
#ifdef PATTERN2
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed);
			slowMode = true;
#else
			attackParam.slowAmout = 0.7f;
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed - attackParam.slowAmout);
			/*if (!e->enemyBaseComponent.anim->AnimEventCan()) {
				e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed + 0.3f);
			}*/
#endif
		}
	}
	if (slowMode) {
		if (e->enemyBaseComponent.anim->EventFinishTime(attackParam.animID) - 5.0f <= e->enemyBaseComponent.anim->GetCurrentFrame()) {
			//attackParam.speedUpMotionSpeed = 0.3f;
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed + attackParam.speedUpMotionSpeed);
			slowMode = false;
		}
	}
}

void BossAttackBase::BossStart()
{
	Boss* boss		= GetBase<Boss>();
	boss->threat	= false;
	//attackParam.flash	= false;
	DebugLogText::GetInstance()->Log(LogLevel::INFO, string + "攻撃が開始しました");
	boss->enemyBaseComponent.anim->Play(attackParam.animID);
	attackTime = boss->enemyBaseComponent.anim->EventFinishTime(attackParam.animID) - boss->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	sound = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	slowMode = false;
}

void BossAttackBase::BossFinish()
{
	Boss* boss = GetBase<Boss>();
	if (boss->maxAttack < 0) {
		boss->threat = true;
	}
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
}


void BossAttackBase::DataSaveAll()
{
	EnemyBase* enemyBase = GetBase<EnemyBase>();
	attackParam.attackCollsionEndTime = enemyBase->enemyBaseComponent.anim->EventFinishTime(attackParam.animID);
	attackParam.attackCollsionStartTime = enemyBase->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	attackParam.justAvoidCollsionStartTime = 5.0f;
	attackParam.justAvoidCollsionEndTime = 6.0f;
	attackParam.attackID = string;
	JSON root;

	root[string.c_str()] = attackParam;

	JsonReader jsonReader;

	jsonReader.Save("data/json/BossAttack/" + string + ".json", root);
}

void BossAttackBase::AttackCollsion()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	//敵の攻撃判定の生成クラス
	if (e->enemyBaseComponent.anim->AnimEventCan()) {
		if (firstColl) {
			firstColl = false;
			e->CollsionStart<SphereCollider>(&e->attackColl, collTrans);
			e->SetShape(CollsionInformation::SPHERE, &e->attackColl);
		}
	}
}

void BossAttackBase::BossAttackCollsion()
{
	Boss* b = GetBase<Boss>();
	//敵の攻撃判定の生成クラス
	if (b->enemyBaseComponent.anim->AnimEventCan()) {
		if (firstColl) {
			firstColl = false;
			b->CollsionStart<SphereCollider>(&b->attackColl, attackParam.attackCollTransform);
			b->SetShape(CollsionInformation::SPHERE, &b->attackColl);
		}
	}
}

void BossAttackBase::AttackSound()
{
	EnemyBase* e = GetBase<EnemyBase>();
	float time = e->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	//敵の攻撃の音を発生
	//attackParam.attackSoundStartTime = 1.0f;
	if (time - attackParam.attackSoundStartTime <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		if (!SoundManager::GetInstance()->CheckSe(Sound_ID::ENEMY_SWORD_WIND1) && !SoundManager::GetInstance()->CheckSe(Sound_ID::ENEMY_SWORD_WIND2)) {
			SoundManager::GetInstance()->RandamSe("EnemySword", 2);
		}
	}
}

void BossAttackBase::AttackFlash(ID::IDType _modelId, int _modelFrame, std::string _voice)
{
	EnemyBase* e = GetBase<EnemyBase>();
	float time = e->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	if (!attackParam.useFlash) {
		return;
	}
	//敵の剣回りを光らせていることへの設定
	//attackParam.attackFlashStartTime = 7.0f;
	///attackParam.attackPositionFrameNum = _modelFrame;
	//attackParam.voiceName = _voice;
	if (time - attackParam.attackFlashStartTime <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		if (_modelId == ID::E_MODEL) {
			int a = 0;
		}
		AttackBeforeFrash(_modelId, _modelFrame, _voice);
		//com.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(0, 0, 300) * MGetRotY(com.enemy->GetEnemyTransform()->rotation.y), 100.0f, 10.0f, 200.0f, 255.0f, 28, 1.0f);
	}
}

void BossAttackBase::AttackBeforeFrash(ID::IDType _modelId, int _modelFrame, std::string _voice)
{
	EnemyBase* e = GetBase<EnemyBase>();
	if (sound) {

		MATRIX matrixWorld = MV1GetFrameLocalWorldMatrix(ResourceLoad::GetHandle(_modelId), _modelFrame);
		// WORLD座標を取得
		VECTOR3 frameWorldPos = MV1GetFramePosition(ResourceLoad::GetHandle(_modelId), _modelFrame);


		MATRIX objWorldMat = obj->GetTransform()->WorldTransform().GetMatrix();
		MATRIX invObjWorldMat = MInverse(objWorldMat);
		VECTOR3 frashPosLocal = frameWorldPos * invObjWorldMat;
		//e->enemyBaseComponent.effect->CreateEffekseer(Transform(frashPos_local, VZero, VOne), obj, Effect_ID::ENEMY_FLASH, 1.0f);
		if (_modelId == ID::E_MODEL) {
			EffectManager::GetInstance()->CreateEffekseer(Transform(frameWorldPos + obj->GetTransform()->position, VZero, VOne), nullptr, Effect_ID::ENEMY_FLASH, 1.0f);
		}
		else {
			EffectManager::GetInstance()->CreateEffekseer(Transform(frashPosLocal, VZero, VOne), obj, Effect_ID::ENEMY_FLASH, 1.0f);
		}

		SoundManager::GetInstance()->PlaySe(Sound_ID::ENEMY_ATTACK_BEFORE);
		SoundManager::GetInstance()->RandamSe(_voice, 3);
		//com.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(500, 500, 1000) * MGetRotY(com.enemy->GetEnemyTransform()->rotation.y), 100.0f, 10.0f, 200.0f, 255.0f, 28, 0.5f);
		sound = false;
		keepAnimSpeed = e->enemyBaseComponent.anim->GetPlaySpeed();
		//attackParam.slowAmout = 0.1f;
		e->enemyBaseComponent.anim->SetPlaySpeed(attackParam.slowAmout);
		//attackParam.slowTime = 0.3f;
		animSlowCounter = attackParam.slowTime;

	}
}

void BossAttackBase::Trail()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	float time = e->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	//剣の軌跡を発生
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time + 5.0f >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		e->Trail();
	}
}

void BossAttackBase::BossTrail(bool _right)
{
	Boss* e = GetBase<Boss>();
	float time = e->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	//剣の軌跡を発生
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time + 5.0f >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		e->Drail(_right);
	}
}

void BossAttackBase::BossJustAvoidCollsion()
{
	Boss* e = GetBase<Boss>();
	float time = e->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	//ジャスト回避判定の作成
	if (time - attackParam.justAvoidCollsionStartTime <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		//Transform colTrans = attackParam.attackCollTransform;
		//colTrans.scale.x += 200.0f;
		//attackParam.justAvoidCollTransform = colTrans;
		e->CollsionStart<SphereCollider>(&e->justAvoidColl, attackParam.justAvoidCollTransform);
		e->SetShape(CollsionInformation::SPHERE, &e->justAvoidColl);
		e->justAvoidCollTime = 3.0f;
	}
}

void BossAttackBase::EnemyJustAvoidCollsion()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	float time = e->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	//ジャスト回避判定の作成
	if (time - 6.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		Transform colTrans = collTrans;
		colTrans.scale.x += 200.0f;
		e->CollsionStart<SphereCollider>(&e->justAvoidColl, colTrans);
		e->SetShape(CollsionInformation::SPHERE, &e->justAvoidColl);
		e->justAvoidCollTime = 3.0f;
	}
}

void BossAttackBase::LoadAttackParam()
{
	JSON root;
	JsonReader jsonReader;

	std::string path = "data/json/BossAttack/" + string + ".json";

	if (!jsonReader.Load(path))
	{
		return;
	}

	root = jsonReader.Data();

	if (root.contains(string))
	{
		attackParam = root[string].get<BossAttackParam>();
	}
	else if (!root.empty())
	{
		// フォールバック
		attackParam = root.begin().value().get<BossAttackParam>();
	}

	// 念のためID補完
	if (attackParam.attackID.empty())
	{
		attackParam.attackID = string;
	}
}
