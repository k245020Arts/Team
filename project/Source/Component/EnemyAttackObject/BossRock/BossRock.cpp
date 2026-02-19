#include "BossRock.h"
#include "../../Collider/SphereCollider.h"
#include "../../Collider/RayCollider.h"
#include "../../Collider/ModelCollider.h"
#include "../../MeshRenderer/MeshRenderer.h"
#include "../../ComponentManager.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/LoadManager.h"
#include "../../Physics/Physics.h"

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
}

void BossRock::Update()
{
	time -= Time::DeltaTimeRate();
	//obj->GetTransform()->rotation.z += 1.0f * DegToRad;
	if (time <= 0.0f) {
		obj->DestroyMe();
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
	preTransform = Transform(_pos,VZero,VECTOR3(5.0f,0.1f,5.0f));
	preDraw = true;
}

void BossRock::Ground()
{
	if (fly) {
		obj->DestroyMe();
	}
	if (groundInit) {
		return;
	}
	groundInit = true;
	obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("_rockAttack");
	obj->Component()->RemoveComponentWithTagIsCollsion<RayCollider>("uiColl");
	randColl = nullptr;
	playerHitColl = nullptr;
	preDraw = false;

	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::MODEL;
	info.oneColl = false;
	info.tag = CollsionInformation::BOSS_ROCK;

	pushColl = obj->Component()->AddComponent<ModelCollider>();
	switch (useHandleNumber)
	{
	case 0:
		pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 2.0f), MV1DuplicateModel(Load::GetHandle(ID::BOSS_PUSH)));
		break;
	case 1:
		pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 2.0f), MV1DuplicateModel(Load::GetHandle(ID::BOSS_PUSH)));
		break;
	case 2:
		pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 4.0f), MV1DuplicateModel(Load::GetHandle(ID::BOSS_PUSH)));
		break;
	}

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;

	playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
	playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "bossplayerAttack");

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
