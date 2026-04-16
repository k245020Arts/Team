#include "BossRockBase.h"
#include "BossRockManager.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Common/ResourceLoader.h"
#include "../../Collider/SphereCollider.h"
#include "../../Collider/RayCollider.h"
#include "../../Collider/ModelCollider.h"
#include "../../Collider/DountCollider.h"
#include "../../ComponentManager.h"
#include "../../Physics/Physics.h"
#include "../../Shaker/Shaker.h"
#include "../../MeshRenderer/MeshRenderer.h"

BossRockBase::BossRockBase()
{
	randColl = nullptr;
	playerHitColl = nullptr;
	bossHitColl = nullptr;
	uiColl = nullptr;
	pushColl = nullptr;
	playerAttackHitColl = nullptr;
	bossRushHitColl = nullptr;
	blastColl = nullptr;
	physics = nullptr; 
	blastJustAvoidColl = nullptr;

	blast = false;
	color = nullptr;
	blinkCounter = 0.0f;
	blinkBaseMax = 0.5f;
	ChangeColorMode = false;
	nowBlast = false;
	flyCounter = 0.0f;

	blastCan = false;
	fly = false;
	groundInit = false;

	rockManager = FindGameObject<BossRockManager>();

	useHandleNumber = -1;

	justAvoidCollider = nullptr;

	preDraw = false;
	preTransform = Transform();
	preModel = ResourceLoad::LoadModel("BossRockPre", ID::BOSS_PRE_MODEL);

	throwRock = false;
	throwReady = false;
	throwRockBaseModel = -1;
	dir = VZero;
	boneNum = 0;
	addPos = VZero;

	debugId = 34;
	tag = Function::GetClassNameC<BossRockBase>();
}

BossRockBase::~BossRockBase()
{
	randColl = nullptr;
	playerHitColl = nullptr;
	bossHitColl = nullptr;
	uiColl = nullptr;
	pushColl = nullptr;
	blastColl = nullptr;
	playerAttackHitColl = nullptr;
	physics = nullptr;
	bossRushHitColl = nullptr;
	useHandleNumber = -1;

	useHandleNumber = -1;
	physics = nullptr;
	rockManager->RemoveList(this);
	justAvoidCollider = nullptr;
	blastJustAvoidColl = nullptr;
	hitObjects.clear();
	preDraw = false;
	throwReady = false;
	playerAttackHit = false;
}

void BossRockBase::Awake()
{
	rockManager->PushList(this);
}

void BossRockBase::Update()
{
	if (throwReady) {
		Transform* transform = obj->GetTransform();
		MATRIX matrixWorld = MV1GetFrameLocalWorldMatrix(throwRockBaseModel, boneNum);
		// WORLD座標を取得
		VECTOR3 frameWorldPos = MV1GetFramePosition(throwRockBaseModel, boneNum);
		MATRIX objWorldMat = obj->GetParent()->GetTransform()->WorldTransform().GetMatrix();
		MATRIX invObjWorldMat = MInverse(objWorldMat);
		VECTOR3 frashPosLocal = frameWorldPos * invObjWorldMat;
		transform->position = frameWorldPos + addPos;
	}
	if (!attackData.groundDelete) {
		groundTime -= Time::DeltaTimeRate();
		if (groundTime <= 0.0f) {
			obj->DestroyMe();
		}
	}
	if (throwRock) {
		if (velocityAdd) {
			VECTOR3 direction = dir + attackData.diffusionAngle;
			physics->AddVelocity(direction * attackData.throwSpeed, true);
			VECTOR3 dist = obj->GetTransform()->position - obj->GetParent()->GetTransform()->position;
			if (dist.Size() >= 50000.0f) {
				obj->DestroyMe();
			}
		}
		
	}
	if (attackData.freeDir) {
		//VECTOR3 direction = dir + attackData.diffusionAngle;
		if (velocityAdd) {
			physics->AddVelocity(attackData.thorwVelocity, true);
		}
	}
	if (playerAttackHit) {

		flyCounter -= Time::DeltaTimeRate();
		if (flyCounter <= 0.0f) {
			fly = true;
		}
	}
	if (nowBlast) {
		//50
		float waveSpeed = attackData.waveSpeed;
		blastColl->GetTransform()->scale.x += waveSpeed;
		blastColl->AddOutRadius(waveSpeed);
		blastJustAvoidColl->GetTransform()->scale.x += waveSpeed;
		blastJustAvoidColl->AddOutRadius(waveSpeed);
		//3000
		if (blastColl->GetOutRadius() >= attackData.maxRadius) {
			obj->DestroyMe();
		}
		if (blastJustAvoidColl->GetOutRadius() >= attackData.maxRadius) {
			obj->DestroyMe();
		}
	}
	else {
		if (blast) {
			blinkCounter -= Time::DeltaTimeRate();
			if (blinkCounter <= 0.0f) {
				blinkCounter = blinkBaseMax;
				ChangeColorMode = !ChangeColorMode;
				blinkBaseMax -= blinkBaseMax / 8.0f;
			}
			if (ChangeColorMode) {
				color->setRGB(Color::Rgb(255, 0, 0, 255));
			}
			else {
				color->setRGB(Color::Rgb(255, 255, 255, 255));
			}
			if (blinkBaseMax <= 0.01f) {
				BlastCollsionCreate();
				
			}
		}
	}
}

void BossRockBase::Draw()
{
	if (preDraw) {
		//MV1SetDifColorScale(preModel, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMatrix(preModel, preTransform.GetMatrix());
		MV1DrawModel(preModel);
	}
}

void BossRockBase::Ground()
{
	if (fly || attackData.groundDelete) {
		if (attackData.blastCan) {
			BlastCollsionCreate();
		}
		else {
			obj->DestroyMe();
			SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BREAK);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VZero, VZero, VOne * 4.0f), obj, Effect_ID::ROCK_BREAK, 1.0f);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0, -100, 0), VZero, VOne * 4.0f), obj, Effect_ID::BOSS_GROUND, 6.0f);
		}
		return;
	}
	if (groundInit || throwReady) {
		return;
	}
	SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_GROUND);
	EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0,-100,0), VZero, VOne * 4.0f), obj, Effect_ID::BOSS_GROUND, 1.0f);
	//effectManager->StopEffekseer(Effect_ID::ROCK_FALL);
	groundInit = true;
	if (!attackData.playerGroundHit) {
		obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("_rockAttack");
		playerHitColl = nullptr;
	}
	
	obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("justAvoid_rock");
	//randColl = nullptr;
	//flyCounter = 0.5f;
	
	velocityAdd = false;
	if (attackData.pushCollCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::MODEL;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK;
		if (pushColl == nullptr) {
			VECTOR3 size = rockManager->GetPushCollSize(attackData);
			pushColl = obj->Component()->AddComponent<ModelCollider>();
			pushColl->ModelColliderSet(info, Transform(VZero, VZero, size), MV1DuplicateModel(ResourceLoad::GetHandle(ID::BOSS_PUSH)));
		}
	}

	if (attackData.playerAttackFlying) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;

		if (playerAttackHitColl == nullptr) {
			playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
			//200
			playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.playerAttackFlyingCollRadius, 1.0f, 1.0f)), "bossplayerAttack");
		}
	}

	if (attackData.bossRushHit) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_RUSH;
		if (bossRushHitColl == nullptr) {
			bossRushHitColl = obj->Component()->AddComponent<SphereCollider>();
			bossRushHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.bossRushHitCollRadius, 1.0f, 1.0f)), "bossRushAttack");
		}
	}

	rockManager->ShakeCamera();
	
	if (attackData.blastCan) {
		
		if (blastCan) {
			blast = true;
		}
	}

	if (attackData.predictionCicleCan) {
		preDraw = false;
		obj->Component()->RemoveComponentWithTagIsCollsion<RayCollider>("uiColl");
		uiColl = nullptr;
	}

	/*if (pushColl == nullptr) {
		pushColl = obj->Component()->AddComponent<ModelCollider>();
		switch (useHandleNumber)
		{
		case 0:
			pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 4.0f), MV1DuplicateModel(ResourceLoad::GetHandle(ID::BOSS_PUSH)));
			break;
		case 1:
			pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 3.0f), MV1DuplicateModel(ResourceLoad::GetHandle(ID::BOSS_PUSH)));
			break;
		case 2:
			pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 10.0f), MV1DuplicateModel(ResourceLoad::GetHandle(ID::BOSS_PUSH)));
			break;
		}
	}*/
	
	physics->SetVelocity(VZero);
	

	

	
	
	
}


void BossRockBase::PlayerAttackRockFlyAway(Transform& _playerTransform)
{
	CollsionInfo info;

	if (attackData.bossHit) {
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_DAMAGE;

		bossHitColl = obj->Component()->AddComponent<SphereCollider>();
		bossHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.bossHitCollRadius, 1.0f, 1.0f)), "rockBossHit");
	}
	
	physics->SetVelocity(_playerTransform.Forward() * /*10000.0f*/attackData.flyingSpeed);
	physics->AddVelocity(VECTOR3(0, attackData.flyingHeight, 0), false);
	//physics->SetGravity(VZero);
	flyCounter = 0.5f;
	EffectManager::GetInstance()->CreateEffekseer(Transform(obj->GetTransform()->position + VECTOR3(0, 0, -100), VZero, VOne * 2.0f), nullptr, Effect_ID::PLAYER_ATTACK_ROCK, 1.0f);
	obj->Component()->GetComponent<Shaker>()->ShakeStart(VOne * 50.0f, Shaker::MIX_SHAKE, true, 0.3f);
	
	playerAttackHit = true;

	if (playerHitColl != nullptr) {
		playerHitColl = nullptr;
		obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("_rockAttack");
	}
	/*obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("bossplayerAttack");
	playerAttackHitColl = nullptr;*/

}

void BossRockBase::SetRockModel()
{
	/*MeshRenderer* mesh = obj->Component()->AddComponent<MeshRenderer>();

	int random = GetRand(2);

	switch (random)
	{
	case 0:
		mesh->ModelHandle(ResourceLoad::LoadModel(ResourceLoad::MODEL_PATH + "BossAttackRock1", ID::ROCK_1));
		obj->GetTransform()->scale = VOne * 1.0f;
		break;
	case 1:
		mesh->ModelHandle(ResourceLoad::LoadModel(ResourceLoad::MODEL_PATH + "BossAttackRock2", ID::ROCK_2));
		obj->GetTransform()->scale = VOne * 1.0f;
		break;
	case 2:
		mesh->ModelHandle(ResourceLoad::LoadModel(ResourceLoad::MODEL_PATH + "BossAttackRock3", ID::ROCK_3));
		obj->GetTransform()->scale = VOne * 0.5f;
		break;
	default:
		my_error_assert("rockModel設定忘れ");
		break;
	}

	useHandleNumber = random;*/
}

void BossRockBase::RockBossHit()
{
	if (attackData.blastCan) {
		SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BLAST);
		BlastCollsionCreate();
	}
	/*blastColl = obj->Component()->AddComponent<DountCollider>();
	blastJustAvoidColl = obj->Component()->AddComponent<DountCollider>();
	nowBlast = true;
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::DONUT;
	info.oneColl = false;
	info.tag = CollsionInformation::ROCK_BLAST_DAMAGE;
	blastColl->DountSet(info, Transform(VZero, VZero, VOne * 50.0f), 100.0f);
	info.oneColl = false;
	info.tag = CollsionInformation::JUST_AVOID;
	blastJustAvoidColl->DountSet(info, Transform(VZero, VZero, VOne * 500.0f), 850.0f);*/
	
	/*EffectManager::GetInstance()->CreateEffekseer(Transform(obj->GetTransform()->position , VZero, VOne * 3.0f), nullptr, Effect_ID::ROCK_BLAST, 1.0f);*/
}

void BossRockBase::Start(const BossAttackBase::ThrowObjectAttackData& _attack)
{
	attackData = _attack;
	if (attackData.randCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::RAY;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK_F;
		randColl = obj->Component()->AddComponent<RayCollider>();
		//300，-300
		randColl->RaySet(info, Transform(VECTOR3(0, _attack.randCollInfo.rayStartPos, 0), VZero, VOne), Transform(VECTOR3(0, _attack.randCollInfo.rayFinishPos, 0), VZero, VOne));
	}
	if (attackData.playerHit) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_ATTACK;
		playerHitColl = obj->Component()->AddComponent<SphereCollider>();
		//200
		playerHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.playerHitCollRadius, 1.0f, 1.0f)), "_rockAttack");

		info.oneColl = false;
		info.tag = CollsionInformation::JUST_AVOID;
		if (justAvoidCollider == nullptr) {
			justAvoidCollider = obj->Component()->AddComponent<SphereCollider>();
			//450
			justAvoidCollider->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.playerHitCollRadius, 1.0f, 1.0f)), "justAvoid_rock");
		}
	}

	if (attackData.predictionCicleCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::RAY;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK_UI;
		uiColl = obj->Component()->AddComponent<RayCollider>();
		//10000,-10000
		uiColl->RaySet(info, Transform(VECTOR3(0, attackData.predictionCicleColliderInfo.rayStartPos, 0), VZero, VOne), Transform(VECTOR3(0, attackData.predictionCicleColliderInfo.rayFinishPos, 0), VZero, VOne), "uiColl");
	}

	preDraw = false;
	preModel = ResourceLoad::GetHandle(ID::BOSS_PRE_MODEL);
	groundInit = false;

	physics = obj->Component()->GetComponent<Physics>();
	fly = false;
	blast = false;
	color = obj->Component()->GetComponent<Color>();

	blinkBaseMax = attackData.blastBlinkMaxCounter;
	blinkCounter = blinkBaseMax;
	ChangeColorMode = false;
	nowBlast = false;
	if (attackData.throwToFall || attackData.throwToFallToPlayer) {
		SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_FALL);

		EffectManager::GetInstance()->CreateEffekseer(Transform(VZero, VZero, VOne * 2.0f), obj, Effect_ID::ROCK_FALL, 3.0f);
	}
	if (attackData.armThrow) {
		throwReady = true;

		throwRockBaseModel = ResourceLoad::GetHandle(ID::B_MODEL);
		//15
		boneNum = attackData.armFrameNum;
		addPos = attackData.armAddPos;
	}
	if (attackData.freeDir) {
		velocityAdd = true;
	}
	groundTime = attackData.randTime;
}

void BossRockBase::DropObject()
{
	if (throwRock) {
		return;
	}
	throwReady = false;
	if (attackData.randCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::RAY;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK_F;
		if (randColl == nullptr) {
			randColl = obj->Component()->AddComponent<RayCollider>();
			randColl->RaySet(info, Transform(VECTOR3(0, 300, 0), VZero, VOne), Transform(VECTOR3(0, -300, 0), VZero, VOne));
		}
	}
	
	if (attackData.playerAttackFlying) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;

		if (playerAttackHitColl == nullptr) {
			playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
			playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(250.0f, 1.0f, 1.0f)), "bossplayerAttack");
		}
	}
	

	throwRock = true;

	float g = 2000.0f;
	physics->SetGravity(VECTOR3(0, -g, 0));
}



void BossRockBase::ThrowRockStart(BaseObject* _player)
{
	if (throwRock) {
		return;
	}
	dir = obj->GetParent()->GetTransform()->Forward() * -1.0f;
	throwReady = false;
	if (attackData.randCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::RAY;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK_F;
		if (randColl == nullptr) {
			randColl = obj->Component()->AddComponent<RayCollider>();
			randColl->RaySet(info, Transform(VECTOR3(0, 300, 0), VZero, VOne), Transform(VECTOR3(0, -300, 0), VZero, VOne));
		}
	}

	if (attackData.playerAttackFlying) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;

		if (playerAttackHitColl == nullptr) {
			playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
			playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(250.0f, 1.0f, 1.0f)), "bossplayerAttack");
		}
	}


	throwRock = true;
	if (attackData.throwToPlayer) {
		//プレイヤーに向かって飛ぶ
		VECTOR3 start = obj->GetTransform()->position;
		VECTOR3 target = _player->GetTransform()->position;

		VECTOR3 diff = target - start;

		float g = 2000.0f;
		physics->SetGravity(VECTOR3(0, -g, 0));

		VECTOR3 diffXZ = VECTOR3(diff.x, 0, diff.z);
		float distance = diffXZ.Size();

		float height = diff.y;

		float angle = 30.0f * DegToRad;

		float cosA = cosf(angle);
		float sinA = sinf(angle);

		float numerator = g * distance * distance;
		float denominator = 2 * cosA * cosA * (distance * tanf(angle) - height);

		float speed = sqrtf(numerator / denominator);

		// 速度ベクトル
		VECTOR3 dir = diffXZ.Normalize();

		VECTOR3 velocity;
		velocity.x = dir.x * speed * cosA;
		velocity.z = dir.z * speed * cosA;
		velocity.y = speed * sinA;

		physics->AddVelocity(velocity, false);
	}
	if (attackData.thorwToFront) {
		VECTOR3 direction = dir + attackData.diffusionAngle;
		physics->AddVelocity(direction * attackData.throwFirstSpeed, true);
		physics->AddVelocity(VECTOR3(0.0f, attackData.upSpeed, 0.0f), false);

		physics->SetGravity(VECTOR3(0.0f, -attackData.throwFallGravity, 0.0f));
		velocityAdd = true;
	}

}


void BossRockBase::BlastCollsionCreate()
{
	if (blastColl != nullptr) {
		return;
	}
	CollsionInfo info;
	blastColl = obj->Component()->AddComponent<DountCollider>();
	blastJustAvoidColl = obj->Component()->AddComponent<DountCollider>();
	nowBlast = true;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::DONUT;
	info.oneColl = false;
	info.tag = CollsionInformation::ROCK_BLAST_DAMAGE;
	//50,100
	blastColl->DountSet(info, Transform(VZero, VZero, VOne * attackData.blastColliderInfo.inRadius), attackData.blastColliderInfo.outRadius);
	info.oneColl = false;
	info.tag = CollsionInformation::JUST_AVOID;
	//300,950
	blastJustAvoidColl->DountSet(info, Transform(VZero, VZero, VOne * attackData.blastColliderInfo.inRadius), attackData.blastColliderInfo.outRadius);
	SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BLAST);
	SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BLAST);
	EffectManager::GetInstance()->CreateEffekseer(Transform(obj->GetTransform()->position, VZero, VOne * 3.0f), nullptr, Effect_ID::ROCK_BLAST, 1.0f);
}

void BossRockBase::SetPreInfo(const VECTOR3& _pos)
{
	preTransform = Transform(_pos, VZero, VECTOR3(5.0f, 0.1f, 5.0f));
	preDraw = true;
}