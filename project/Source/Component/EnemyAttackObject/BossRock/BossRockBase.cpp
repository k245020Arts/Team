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

	blastCan = false;
	fly = true;
	groundInit = false;

	rockManager = FindGameObject<BossRockManager>();

	useHandleNumber = -1;

	justAvoidCollider = nullptr;
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
}

void BossRockBase::Awake()
{
	rockManager->PushList(this);
}

void BossRockBase::Update()
{
	if (nowBlast) {
		float waveSpeed = 50.0f;
		blastColl->GetTransform()->scale.x += waveSpeed;
		blastColl->AddOutRadius(waveSpeed);
		blastJustAvoidColl->GetTransform()->scale.x += waveSpeed;
		blastJustAvoidColl->AddOutRadius(waveSpeed);
		if (blastColl->GetOutRadius() >= 3000.0f) {
			obj->DestroyMe();
		}
		if (blastJustAvoidColl->GetOutRadius() >= 3000.0f) {
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
				blastColl = obj->Component()->AddComponent<DountCollider>();
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
				blastJustAvoidColl->DountSet(info, Transform(VZero, VZero, VOne * 300.0f), 950.0f);
				SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BLAST);
				SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BLAST);
				EffectManager::GetInstance()->CreateEffekseer(Transform(obj->GetTransform()->position, VZero, VOne * 3.0f), nullptr, Effect_ID::ROCK_BLAST, 1.0f);
			}
		}
	}
}

void BossRockBase::Ground()
{
	if (fly) {
		obj->DestroyMe();
		SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BREAK);
		EffectManager::GetInstance()->CreateEffekseer(Transform(VZero, VZero, VOne * 4.0f), obj, Effect_ID::ROCK_BREAK, 1.0f);
		EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0, -100, 0), VZero, VOne * 4.0f), obj, Effect_ID::BOSS_GROUND, 6.0f);
		return;
	}
	if (groundInit) {
		return;
	}
	SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_GROUND);
	EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0,-100,0), VZero, VOne * 4.0f), obj, Effect_ID::BOSS_GROUND, 1.0f);
	//effectManager->StopEffekseer(Effect_ID::ROCK_FALL);
	groundInit = true;
	obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("_rockAttack");
	obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("justAvoid_rock");
	//randColl = nullptr;
	playerHitColl = nullptr;
	

	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::MODEL;
	info.oneColl = false;
	info.tag = CollsionInformation::BOSS_ROCK;

	if (pushColl == nullptr) {
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
	}
	
	physics->SetVelocity(VZero);
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;

	if (playerAttackHitColl == nullptr) {
		playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
		playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "bossplayerAttack");
	}

	

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_RUSH;
	if (bossRushHitColl == nullptr) {
		bossRushHitColl = obj->Component()->AddComponent<SphereCollider>();
		bossRushHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "bossRushAttack");
	}
	
	rockManager->ShakeCamera();
	if (blastCan) {
		blast = true;
	}
}


void BossRockBase::PlayerAttackRockFlyAway(Transform& _playerTransform)
{
	CollsionInfo info;

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_DAMAGE;

	bossHitColl = obj->Component()->AddComponent<SphereCollider>();
	bossHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "rockBossHit");

	physics->SetVelocity(_playerTransform.Forward() * 10000.0f);
	physics->AddVelocity(VECTOR3(0,1200, 0), false);
	fly = true;
	EffectManager::GetInstance()->CreateEffekseer(Transform(obj->GetTransform()->position + VECTOR3(0, 0, -100), VZero, VOne * 2.0f), nullptr, Effect_ID::PLAYER_ATTACK_ROCK, 1.0f);
	obj->Component()->GetComponent<Shaker>()->ShakeStart(VOne * 50.0f,Shaker::MIX_SHAKE,true,0.3f);

	if (playerHitColl != nullptr) {
		playerHitColl = nullptr;
		obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("_rockAttack");
	}
	/*obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("bossplayerAttack");
	playerAttackHitColl = nullptr;*/

}

void BossRockBase::SetRockModel()
{
	MeshRenderer* mesh = obj->Component()->AddComponent<MeshRenderer>();

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
		my_error_assert("rockModelÝ’è–Y‚ê");
		break;
	}

	useHandleNumber = random;
}

void BossRockBase::RockBossHit()
{
	blastColl = obj->Component()->AddComponent<DountCollider>();
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
	blastJustAvoidColl->DountSet(info, Transform(VZero, VZero, VOne * 500.0f), 850.0f);
	SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BLAST);
	EffectManager::GetInstance()->CreateEffekseer(Transform(obj->GetTransform()->position , VZero, VOne * 3.0f), nullptr, Effect_ID::ROCK_BLAST, 1.0f);
}
