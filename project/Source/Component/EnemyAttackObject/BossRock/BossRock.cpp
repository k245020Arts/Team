#include "BossRock.h"
#include "../../Collider/SphereCollider.h"
#include "../../Collider/RayCollider.h"
#include "../../MeshRenderer/MeshRenderer.h"
#include "../../Transform/Transform.h"
#include "../../ComponentManager.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/LoadManager.h"

BossRock::BossRock()
{
	randColl = nullptr;
	playerHitColl = nullptr;
	bossHitColl = nullptr;

	collTag = CollsionInformation::TAG_MAX;
	time = 5.0f;
	debugId = 33;
	tag = Function::GetClassNameC<BossRock>();
}

BossRock::~BossRock()
{
}

void BossRock::Update()
{
	time -= Time::DeltaTimeRate();
	if (time <= 0.0f) {
		obj->DestroyMe();
	}
}

void BossRock::Draw()
{
}

void BossRock::StartCollAdd(CollsionInformation::Tag _tag, const Transform& _transform)
{
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::RAY;
	info.oneColl = false;
	info.tag = CollsionInformation::BOSS_ROCK_F;
	randColl = obj->Component()->AddComponent<RayCollider>();
	randColl->RaySet(info, Transform(VECTOR3(0, 500, 0), VZero, VOne), Transform(VECTOR3(0, -500, 0), VZero, VOne));

	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = _tag;
	playerHitColl = obj->Component()->AddComponent<SphereCollider>();
	playerHitColl->CollsionAdd(info, Transform());

	colltag = _tag;
	time = 5.0f;
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
		break;
	case 1:
		mesh->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "BossAttackRock2", ID::ROCK_2));
		break;
	case 2:
		mesh->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "BossAttackRock3", ID::ROCK_3));
		break;
	default:
		my_error_assert("rockModelê›íËñYÇÍ");
		break;
	}
}
