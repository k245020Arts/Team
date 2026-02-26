#include "BossThrowRock.h"
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
#include "../../../Common/Random.h"

BossThrowRock::BossThrowRock()
{
	throwRock = false;
	throwReady = true;
	throwRockBaseModel = -1;
	dir = VZero;
	boneNum = 0;
	addPos = VZero;

	debugId = 34;
	tag = Function::GetClassNameC<BossThrowRock>();

}

BossThrowRock::~BossThrowRock()
{
}

void BossThrowRock::Update()
{
	BossRockBase::Update();
	if (throwReady) {
		Transform* transform = obj->GetTransform();
		MATRIX matrixWorld = MV1GetFrameLocalWorldMatrix(throwRockBaseModel, boneNum);
		// WORLDÀ•W‚ðŽæ“¾
		VECTOR3 frameWorldPos = MV1GetFramePosition(throwRockBaseModel, boneNum);
		MATRIX objWorldMat = obj->GetParent()->GetTransform()->WorldTransform().GetMatrix();
		MATRIX invObjWorldMat = MInverse(objWorldMat);
		VECTOR3 frashPosLocal = frameWorldPos * invObjWorldMat;
		transform->position = frameWorldPos + addPos;
	}
}

void BossThrowRock::Draw()
{
}

void BossThrowRock::Ground()
{
	BossRockBase::Ground();
}

void BossThrowRock::CreateThrowRock(VECTOR3& _addPos)
{
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = true;
	info.tag = CollsionInformation::BOSS_ROCK_ATTACK;
	playerHitColl = obj->Component()->AddComponent<SphereCollider>();
	playerHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(200.0f, 1.0f, 1.0f)), "_rockAttack");

	groundInit = false;

	physics = obj->Component()->GetComponent<Physics>();
	fly = false;
	blast = false;
	color = obj->Component()->GetComponent<Color>();

	blinkBaseMax = 0.5f;
	blinkCounter = blinkBaseMax;
	ChangeColorMode = false;
	nowBlast = false;
	throwReady = true;

	throwRockBaseModel = Load::GetHandle(ID::B_MODEL);
	boneNum = 15;
	addPos = _addPos;
}

void BossThrowRock::ThrowRockStart()
{
	if (throwRock) {
		return;
	}
	dir = obj->GetParent()->GetTransform()->rotation.Normalize();
	throwReady = false;
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::RAY;
	info.oneColl = false;
	info.tag = CollsionInformation::BOSS_ROCK_F;
	randColl = obj->Component()->AddComponent<RayCollider>();
	randColl->RaySet(info, Transform(VECTOR3(0, 300, 0), VZero, VOne), Transform(VECTOR3(0, -300, 0), VZero, VOne));

	throwRock = true;
	VECTOR3 gravity = VECTOR3(0, -2000.0f, 0);
	gravity.y = Random::GetFloat(1500.0f, 3000.0f);
	gravity.y *= -1.0f;
	physics->AddGravity(gravity);
	physics->AddVelocity(dir * 5000.0f, false);
	physics->AddVelocity(VECTOR3(0,10000,0), false);
}
