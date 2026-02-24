#include "BossRock.h"
#include "../../Collider/SphereCollider.h"
#include "../../Collider/RayCollider.h"
#include "../../Collider/ModelCollider.h"
#include "../../Collider/DountCollider.h"
#include "../../MeshRenderer/MeshRenderer.h"
#include "../../ComponentManager.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Common/LoadManager.h"
#include "../../Physics/Physics.h"
#include "BossRockManager.h"
#include "../../../Common/Effect/EffectManager.h"

BossRock::BossRock()
{
	randColl = nullptr;
	playerHitColl = nullptr;
	bossHitColl = nullptr;

	collTag = CollsionInformation::TAG_MAX;
	time = 5.0f;
	debugId = 33;
	tag = Function::GetClassNameC<BossRock>();

	preDraw = false;
	uiColl = nullptr;
	preTransform = Transform();
	preModel = Load::LoadModel(Load::MODEL_PATH + "BossRockPre",ID::BOSS_PRE_MODEL);
	groundInit = false;
	pushColl = nullptr;
	useHandleNumber = -1;
	playerAttackHitColl = nullptr;
	fly = true;
	rockManager = FindGameObject<BossRockManager>();
	physics = nullptr;
	bossRushHitColl = nullptr;
	blast = false;
	color = nullptr;
	blinkCounter = 0.0f;
	blinkBaseMax = 0.5f;
	ChangeColorMode = false;
	soundManager = FindGameObject<SoundManager>();
	effectManager = FindGameObject<EffectManager>();
	blastCan = false;
}

BossRock::~BossRock()
{
	randColl = nullptr;
	playerHitColl = nullptr;
	bossHitColl = nullptr;
	uiColl = nullptr;
	pushColl = nullptr;
	useHandleNumber = -1;
	playerAttackHitColl = nullptr;
	physics = nullptr;
	bossRushHitColl = nullptr;
	rockManager->RemoveList(this);
	hitObjects.clear();
}

void BossRock::Update()
{
	if (nowBlast) {
		float waveSpeed = 50.0f;
		blastColl->GetTransform()->scale.x += waveSpeed;
		blastColl->AddOutRadius(waveSpeed);
		if (blastColl->GetOutRadius() >= 3000.0f) {
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
				nowBlast = true;
				CollsionInfo info;
				info.parentTransfrom = obj->GetTransform();
				info.shape = CollsionInformation::DONUT;
				info.oneColl = false;
				info.tag = CollsionInformation::ROCK_BLAST_DAMAGE;
				blastColl->DountSet(info, Transform(VZero,VZero,VOne * 50.0f), 100.0f);
				soundManager->PlaySe(Sound_ID::ROCK_BLAST);
				effectManager->CreateEffekseer(Transform(obj->GetTransform()->position, VZero, VOne * 3.0f), nullptr, Effect_ID::ROCK_BLAST, 1.0f);
			}
		}
	}
}

void BossRock::Draw()
{
	if (preDraw) {
		//MV1SetDifColorScale(preModel, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMatrix(preModel, preTransform.GetMatrix());
		MV1DrawModel(preModel);
	}
}

void BossRock::StartCollAdd(CollsionInformation::Tag _tag, const Transform& _transform)
{
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::RAY;
	info.oneColl = false;
	info.tag = CollsionInformation::BOSS_ROCK_F;
	randColl = obj->Component()->AddComponent<RayCollider>();
	randColl->RaySet(info, Transform(VECTOR3(0, 300, 0), VZero, VOne), Transform(VECTOR3(0, -300, 0), VZero, VOne));

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = _tag;
	playerHitColl = obj->Component()->AddComponent<SphereCollider>();
	playerHitColl->CollsionAdd(info, Transform(VZero,VZero,VECTOR3(200.0f,1.0f,1.0f)),"_rockAttack");

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::RAY;
	info.oneColl = false;
	info.tag = CollsionInformation::BOSS_ROCK_UI;
	uiColl = obj->Component()->AddComponent<RayCollider>();
	uiColl->RaySet(info, Transform(VECTOR3(0, 10000, 0), VZero, VOne), Transform(VECTOR3(0, -10000, 0), VZero, VOne),"uiColl");

	colltag = _tag;
	time = 5.0f;
	preDraw = false;
	preModel = Load::GetHandle(ID::BOSS_PRE_MODEL);
	groundInit = false;

	physics = obj->Component()->GetComponent<Physics>();
	rockManager->PushList(this);
	fly = false;
	blast = false;
	color = obj->Component()->GetComponent<Color>();
	
	blinkBaseMax = 0.5f;
	blinkCounter = blinkBaseMax;
	ChangeColorMode = false;
	nowBlast = false;
	soundManager->PlaySe(Sound_ID::ROCK_FALL);

	effectManager->CreateEffekseer(Transform(VZero, VZero, VOne * 2.0f), obj, Effect_ID::ROCK_FALL, 3.0f);
}

void BossRock::RemoveCollider(std::string _name)
{

}

void BossRock::SetRockModel()
{
	MeshRenderer* mesh = obj->Component()->AddComponent<MeshRenderer>();

	int random = GetRand(2);
	

	switch (random)
	{
	case 0 :
		mesh->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "BossAttackRock1",ID::ROCK_1));
		obj->GetTransform()->scale = VOne * 1.0f;
		break;
	case 1:
		mesh->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "BossAttackRock2", ID::ROCK_2));
		obj->GetTransform()->scale = VOne * 1.0f;
		break;
	case 2:
		mesh->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "BossAttackRock3", ID::ROCK_3));
		obj->GetTransform()->scale = VOne * 0.5f;
		break;
	default:
		my_error_assert("rockModelÝ’è–Y‚ê");
		break;
	}

	useHandleNumber = random;
}

void BossRock::SetPreInfo(const VECTOR3& _pos)
{
	if (preDraw) {
		return;
	}
	preTransform = Transform(_pos,VZero,VECTOR3(5.0f,0.1f,5.0f));
	preDraw = true;
}

void BossRock::Ground()
{
	if (fly) {
		obj->DestroyMe();
		soundManager->PlaySe(Sound_ID::ROCK_BREAK);
		effectManager->CreateEffekseer(Transform(VZero, VZero, VOne), obj, Effect_ID::ROCK_BREAK, 1.0f);
		return;
	}
	if (groundInit) {
		return;
	}
	soundManager->PlaySe(Sound_ID::ROCK_GROUND);
	effectManager->CreateEffekseer(Transform(VZero, VZero, VOne), obj, Effect_ID::BOSS_GROUND, 6.0f);
	//effectManager->StopEffekseer(Effect_ID::ROCK_FALL);
	groundInit = true;
	obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("_rockAttack");
	obj->Component()->RemoveComponentWithTagIsCollsion<RayCollider>("uiColl");
	randColl = nullptr;
	playerHitColl = nullptr;
	preDraw = false;
	physics->SetVelocity(VZero);

	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::MODEL;
	info.oneColl = false;
	info.tag = CollsionInformation::BOSS_ROCK;

	pushColl = obj->Component()->AddComponent<ModelCollider>();
	switch (useHandleNumber)
	{
	case 0:
		pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 4.0f), MV1DuplicateModel(Load::GetHandle(ID::BOSS_PUSH)));
		break;
	case 1:
		pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 3.0f), MV1DuplicateModel(Load::GetHandle(ID::BOSS_PUSH)));
		break;
	case 2:
		pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 10.0f), MV1DuplicateModel(Load::GetHandle(ID::BOSS_PUSH)));
		break;
	}

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;

	playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
	playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "bossplayerAttack");

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_RUSH;

	bossRushHitColl = obj->Component()->AddComponent<SphereCollider>();
	bossRushHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "bossRushAttack");
	rockManager->ShakeCamera();
	if (blastCan) {
		blast = true;
	}
}

void BossRock::PlayerAttackRockFlyAway(Transform& _playerTransform)
{
	CollsionInfo info;

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_DAMAGE;

	bossHitColl = obj->Component()->AddComponent<SphereCollider>();
	bossHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "rockBossHit");

	physics->SetVelocity(_playerTransform.Forward() * 10000.0f);
	physics->AddVelocity(VECTOR3(0, 1000, 0), false);
	fly = true;

	/*obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("bossplayerAttack");
	playerAttackHitColl = nullptr;*/

}
