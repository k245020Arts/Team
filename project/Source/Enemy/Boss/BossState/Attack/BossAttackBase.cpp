#include "BossAttackBase.h"
#include "../../Boss.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../CharaBase/CharaBase.h"
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
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"
#include "../../../../Component/Physics/Physics.h"
#include "../../../../Component/Collider/DountCollider.h"
#include "../../../../Component/EnemyAttackObject/ShockWave/ShockWave.h"
#include "../../../../Component/EnemyAttackObject/BossRock/BossRockManager.h"
#include "../AttackSorting.h"

#define PATTERN2

BossAttackBase::BossAttackBase()
{
	attackParam = BossAttackParam();
	averageSpeed = 0.0f;
	normal = VZero;
	aloowStop = false;
	firstJump = false;
	gravitySpeed = false;
	groundEffect = false;
	rockGet = false;
	throwRock = false;

	rushSound = false;
	firstOnes = false;
	secondOnes = false;
	firstMove = false;

	rockColl = nullptr;
	rushAttackCount = 0.0f;
	throwObjectAppearTime = 0.0f;
	throwObjectNumNow = 0;
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
	BossUpdate();
	
}

void BossAttackBase::BossStart()
{
	Boss* boss		= GetBase<Boss>();
	boss->threat	= false;
	//attackParam.flash	= false;
	//DebugLogText::GetInstance()->Log(LogLevel::INFO, string + "攻撃が開始しました");
	if (attackParam.attackBeforeAnimID == ID::ID_MAX) {
		boss->enemyBaseComponent.anim->Play(attackParam.animID);
	}
	else {
		boss->enemyBaseComponent.anim->Play(attackParam.attackBeforeAnimID);
	}

	firstColl = true;
	attackTime = boss->enemyBaseComponent.anim->EventFinishTime(attackParam.animID) - boss->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	sound = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	slowMode = false;
	aloowStop = false;
	firstJump = true;
	gravitySpeed = false;
	groundEffect = true;
	rockGet = false;
	throwRock = false;
	throwObjectAppearTime = attackParam.intervalTime;
	throwObjectNumNow = 0;

	rushSound = false;
	firstOnes = false;
	secondOnes = false;
	firstMove = true;

	rockColl = nullptr;
	boss->threat = false;

	if (attackParam.attackCameraBossLook) {
		boss->enemyBaseComponent.camera->AttackEnemyFovChange(boss->bossTransform, attackParam.cameraChangeSpeed);
	}
}

void BossAttackBase::BossFinish()
{
	Boss* boss = GetBase<Boss>();
	if (boss->maxAttack < 0) {
		boss->threat = true;
	}
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);
	boss->enemyBaseComponent.physics->SetGravity(VECTOR3(0, -1500, 0));

	if (boss->attackColl.instance != nullptr) {
		boss->DeleteCollision(&boss->attackColl);
	}
	boss->enemyBaseComponent.anim->AnimEventReset();
	if (rockColl != nullptr) {
		rockColl->GetBaseObject()->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("Rush");
		rockColl = nullptr;
	}
	for (auto data : attackParam.throwAttackData) {
		if (data.playerAttackObjectDrop) {
			boss->rockManager->DropRockStart();
		}
	}
	firstColl = false;
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
	if (!CurrentAttackAnim()) {
		return;
	}
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
	if (!CurrentAttackAnim()) {
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
		//attackParam.slowTime = 0.3f;
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
	if (!CurrentAttackAnim()) {
		return;
	}
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

void BossAttackBase::SetAttackParam(BossAttackParam _param)
{
	attackParam = _param;
}

void BossAttackBase::RotateEvent()
{
	Boss* boss = GetBase<Boss>();
	if (!attackParam.rotateMove) {
		return;
	}
	if (boss->enemyBaseComponent.anim->AnimEventCan()) {
		//攻撃にかかる時間で90°回したいので１フレームごとに進む角度を求めている。
		averageSpeed = 90.0f / attackTime;
		averageSpeed *= boss->obj->GetObjectTimeRate();

		boss->bossTransform->rotation.y += averageSpeed * DegToRad;
	}
}

void BossAttackBase::LookEvent()
{
	Boss* boss = GetBase<Boss>();
	if (!attackParam.lookPlayer) {
		return;
	}
	if (attackParam.lookMaxCounter <= boss->enemyBaseComponent.anim->GetCurrentFrame()) {
		return;
	}

	for (int i = 0; i < attackParam.lookNum; i++) {
		boss->LookPlayer();
	}
}

void BossAttackBase::MoveEvent()
{
	Boss* boss = GetBase<Boss>();
	if (!CurrentAttackAnim()) {
		return;
	}
	if (!attackParam.playerAloowMove && !attackParam.frontMove) {
		return;
	}
	float animFrame = boss->enemyBaseComponent.anim->GetCurrentFrame();
	if (attackParam.moveStartTime <= animFrame && attackParam.moveFinishTime >= animFrame) {
		if (attackParam.playerAloowMove) {
			if (firstMove) {
				VECTOR3 dis = boss->enemyBaseComponent.playerObj->GetTransform()->position - boss->bossTransform->position;
				normal = dis.Normalize();
				boss->enemyBaseComponent.physics->AddVelocity(normal * attackParam.baseFirstSpeed, false);
				firstMove = false;
				return;//最初の移動の時は返す
			}
			if (aloowStop) {
				VECTOR3 dis = boss->bossTransform->Forward() * 1.0f;
				normal = dis.Normalize();
				normal.y = 0.0f;
				float speed = attackParam.baseSpeed;
				if (attackParam.addVelocity) {
					boss->enemyBaseComponent.physics->AddVelocity(normal * speed, true);
				}
				else {
					boss->enemyBaseComponent.physics->SetVelocity(normal * speed);
				}
				return;
			}
			VECTOR3 dis = boss->enemyBaseComponent.playerObj->GetTransform()->position - boss->bossTransform->position;
			normal = dis.Normalize();
			//y座標をいじりたくないので0にする。
			normal.y = 0.0f;
			bool move = true;
			if (dis.Size() <= attackParam.playerBaseNear) {
				if (attackParam.playerNearStop) {
					boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 24.0f);
					move = false;
				}
				else if (attackParam.playerNearAloowStop) {
					//move = false;
					aloowStop = true;
				}

			}
			if (move) {
				float speed = dis.Size();
				speed = std::clamp(speed, attackParam.minMoveSpeed, attackParam.maxMoveSpeed);
				if (attackParam.addVelocity) {
					boss->enemyBaseComponent.physics->AddVelocity(normal * speed,true);
				}
				else {
					boss->enemyBaseComponent.physics->SetVelocity(normal * speed);
				}
				
			}
			LookEvent();
		}
		if (attackParam.frontMove) {
			if (firstMove) {
				VECTOR3 dis = boss->bossTransform->Forward() * 1.0f;
				normal = dis.Normalize();
				boss->enemyBaseComponent.physics->AddVelocity(normal * attackParam.baseFirstSpeed, false);
				firstMove = false;
				return;//最初の移動の時は返す
			}
			VECTOR3 dis = boss->bossTransform->Forward() * 1.0f;
			normal = dis.Normalize();
			normal.y = 0.0f;
			float speed = attackParam.baseSpeed;
			if (attackParam.addVelocity) {
				boss->enemyBaseComponent.physics->AddVelocity(normal * speed, true);
			}
			else {
				boss->enemyBaseComponent.physics->SetVelocity(normal * speed);
			}
		}

	}
	if (animFrame > attackParam.moveFinishTime) {
		boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 24.0f);
	}
}

void BossAttackBase::JumpEvent()
{
	if (!attackParam.jump) {
		return;
	}
	Boss* boss = GetBase<Boss>();
	if (boss->enemyBaseComponent.anim->GetCurrentFrame() <= attackParam.jumpStartTime) {
		return;
	}
	if (firstJump) {
		firstJump = false;
		boss->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, attackParam.jumpSpeed/*3000.0f*/, 0), false);
	}

	boss->enemyBaseComponent.physics->AddGravity(VECTOR3(0, gravitySpeed, 0));
	gravitySpeed += attackParam.addGravity;

	if (boss->enemyBaseComponent.physics->GetGround()) {
		if (boss->enemyBaseComponent.anim->GetCurrentFrame() >= attackParam.groundEffectStartTime) {
			if (groundEffect) {
				groundEffect = false;
				
				BaseObject* obj2 = EffectManager::GetInstance()->CreateEffekseer(*boss->GetBaseObject()->GetTransform(), boss->GetBaseObject(), attackParam.jumpGroundEffect, 1.0f);
			
				
				EffectManager::GetInstance()->ParentTransformRemove(obj2);
				
				SoundManager::GetInstance()->PlaySe(Sound_ID::GROUND);
				boss->enemyBaseComponent.camera->CameraPerspectiveShakeStart(/*3.0f, 0.4f*/ attackParam.groundShakeCamera,attackParam.groundShakeTime);
				CreateWave();
			}
		}

		/*if (b->enemyBaseComponent.anim->IsFinish()) {
			b->BossAttackStateChange();
		}*/
	}
}

void BossAttackBase::ShackWaveEvent()
{
	if (!attackParam.shockWave) {
		return;
	}
	Boss* boss = GetBase<Boss>();
	if (boss->enemyBaseComponent.anim->AnimEventCan()) {
		if (groundEffect) {
			CreateWave();
			groundEffect = false;
		}
		
	}
}

void BossAttackBase::CreateWave()
{
	if (!attackParam.shockWave) {
		return;
	}
	Boss* boss = GetBase<Boss>();
	BaseObject* obj1 = EffectManager::GetInstance()->CreateEffekseer(*boss->GetBaseObject()->GetTransform(), boss->GetBaseObject(), /*Effect_ID::BOSS_WAVE*/attackParam.shockMoveEffect, 1.0f);
	EffectManager::GetInstance()->ParentTransformRemove(obj1);
	ShockWave* w = obj1->Component()->AddComponent<ShockWave>();
	w->CreateWave(CollsionInformation::B_E_ATTACK, Transform(VZero, VZero, VOne), /*50.0f, 50.0f*/attackParam.startRange,attackParam.shockWaveSpeed);

}

void BossAttackBase::ThrowObjectsEvent()
{
	//投擲物イベントがないなら
	if (!attackParam.throwObject) {
		return;
	}
	Boss* boss = GetBase<Boss>();
	//腕で投げるなら
	if (attackParam.armThrow) {
		float animFrame = boss->enemyBaseComponent.anim->GetCurrentFrame();
		//投げるの開始
		if (attackParam.throwStartTime <= animFrame) {
			if (!throwRock) {
				boss->rockManager->ThrowStart();
				throwRock = true;
			}
		}
		//投擲物が登場するタイミング
		if (animFrame >= attackParam.throwObjectApperaTime) {
			if (!rockGet) {
				boss->rockManager->CreateThrowObject(attackParam.throwAttackData, 0, 0, 0.0f);
				rockGet = true;
			}
		}
	}
	else {
		throwObjectAppearTime -= Time::DeltaTimeRate();
		//岩が登場する時間が0になっているときかつ、MAXの値まで出てないときは登場
		if (throwObjectAppearTime <= 0.0f && throwObjectNumNow < attackParam.throwObjectNum)
		{
			boss->rockManager->CreateThrowObject(attackParam.throwAttackData,throwObjectNumNow, attackParam.throwObjectNum, 0.0f);

			throwObjectNumNow++;

			// 徐々に出るスピードが早くなる
			if (attackParam.intervalTimeSub) {
				float rate = (float)throwObjectNumNow / (float)attackParam.throwObjectNum;
				throwObjectAppearTime = Easing::Lerp(/*0.45f, 0.15f*/attackParam.maxIntervalTime, attackParam.minIntervalTime, rate);
			}
			else {
				throwObjectAppearTime = attackParam.intervalTime;
			}
			
		}
		//投擲物が全部出きったら
		if (throwObjectNumNow >= attackParam.throwObjectNum)
		{
			boss->rockManager->ShakeCamera();
			boss->enemyBaseComponent.state->ChangeState(StateID::BOSS_IDOL_S);
		}
	}
}

void BossAttackBase::RushEvent()
{
	//突進イベントがないなら
	if (!attackParam.rushMove) {
		return;
	}
	Boss* boss = GetBase<Boss>();
	//後隙の時にアニメーションの再生が終わったら終了
	if (boss->enemyBaseComponent.anim->IsFinish()) {
		if (boss->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(attackParam.rushAfterAnimID)) {
			boss->BossAttackStateChange();
		}
		else {
			AttackStart();
		}
	}
	//前隙の時にプレイヤーを見る
	if (boss->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(attackParam.attackBeforeAnimID)) {
		boss->LookPlayer(0.09f);
		return;//前隙の時に後ろの処理に行かないように
	}
	BossDushSound();

	rushAttackCount -= obj->GetObjectTimeRate();
	//突進時間が過ぎたなら後隙へと移行
	if (rushAttackCount <= 0.0f) {
		boss->enemyBaseComponent.anim->Play(attackParam.rushAfterAnimID);
		VECTOR3 p = boss->enemyBaseComponent.physics->GetVelocity().Normalize();
		boss->enemyBaseComponent.physics->AddVelocity(p * -attackParam.rushAfterSpeed/*5000*/, true);
	}
}

bool BossAttackBase::CurrentAttackAnim()
{
	Boss* boss = GetBase<Boss>();
	//攻撃のフレームならTrueを返す(前隙、後隙はfalse)
	return ID::GetID(attackParam.animID) == boss->enemyBaseComponent.anim->GetCurrentID();
}

void BossAttackBase::AttackFinish()
{
	Boss* boss = GetBase<Boss>();
	//攻撃のアニメーションじゃなかったらリターン
	if (!CurrentAttackAnim()) {
		return;
	}
	if (boss->enemyBaseComponent.anim->IsFinish())
	{
		boss->BossAttackStateChange();
		boss->GetStateManager()->GetState<AttackSorting>()->AttackFinish();
	}
}


void BossAttackBase::BossDushSound()
{
	Boss* b = GetBase<Boss>();
	SoundManager::GetInstance()->Play3DSound(Sound_ID::BOSS_WALK, obj, 200000, 30000);
	//突進時の足跡の音(アニメーションフレームで再生)
	if (b->enemyBaseComponent.anim->GetCurrentFrame() >= attackParam.rushSoundLeftFoot &&
		b->enemyBaseComponent.anim->GetCurrentFrame() <= attackParam.rushSoundLeftFoot + 1.0f) {
		//一回だけ鳴らす
		if (firstOnes) {
			rushSound = true;
		}
		firstOnes = false;
	}
	//突進時の足跡の音(アニメーションフレームで再生)
	if (b->enemyBaseComponent.anim->GetCurrentFrame() >= attackParam.rushSoundRightFoot &&
		b->enemyBaseComponent.anim->GetCurrentFrame() <= attackParam.rushSoundRightFoot + 1.0f) {
		//一回だけ鳴らす
		if (secondOnes) {
			rushSound = true;
		}
		secondOnes = false;
	}
	//音がなった時に周波数を変える
	if (rushSound) {
		SoundManager::GetInstance()->PlayRamdomChangeFrequencySe(Sound_ID::BOSS_WALK, 30000, 1000);
		rushSound = false;
		//Debug::DebugLog("bossDushSound");
	}
	float resetTime = 0.0f;
	//右のタイムと左のタイムで比較をして大きい方をResetTimeとする
	if (attackParam.rushSoundRightFoot > attackParam.rushSoundLeftFoot) {
		resetTime = attackParam.rushSoundRightFoot;
	}
	else {
		resetTime = attackParam.rushSoundLeftFoot;
	}
	//最後に音がなった時からしばらくたったら
	if (b->enemyBaseComponent.anim->GetCurrentFrame() >= resetTime + 2.0f) {
		firstOnes = true;
		secondOnes = true;
	}
}

void BossAttackBase::BossUpdate()
{
	Boss* boss = GetBase<Boss>();
	//ボスのポインタが取得できなければイベントを走らせない
	if (boss == nullptr) {
		return;
	}
	AttackFinish();
	AttackSound();
	BossAttackCollsion();
	BossJustAvoidCollsion();
	RotateEvent();
	LookEvent();
	ShackWaveEvent();
	MoveEvent();
	RushEvent();
	JumpEvent();
	ThrowObjectsEvent();
	if (attackParam.useTrail) {
		BossTrail(attackParam.trailRightHand);
	}
	AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
}

void BossAttackBase::AttackStart()
{
	Boss* b = GetBase<Boss>();
	b->enemyBaseComponent.anim->Play(attackParam.animID);

	VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;

	VECTOR3 angle = pos - b->GetBaseObject()->GetTransform()->position;
	//rotation = angle.Normalize();
	firstColl = true;
	//look = true;
	//distance = pos.Size();
	

	//突進した時専用の当たり判定を付けるか
	if (attackParam.rushColl) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_RUSH;

		rockColl = obj->Component()->AddComponent<SphereCollider>();
		Transform rushColl = attackParam.attackCollTransform;
		rushColl.scale.x += attackParam.addRushCollScale;
		rockColl->CollsionAdd(info, rushColl, "Rush");
	}
	//連続攻撃をしないなら
	if (b->maxAttack <= 0) {
		AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
		//attackParam.flash = true;
		rushAttackCount = attackParam.rushTime;
		b->enemyBaseComponent.anim->SetPlaySpeed(2.0f);
	}
	else {
		rushAttackCount = attackParam.rushTime;
		b->enemyBaseComponent.anim->SetPlaySpeed(2.0f);
	}
	firstOnes = true;
	secondOnes = true;
}