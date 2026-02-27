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

BossRock::BossRock()
{
	collTag = CollsionInformation::TAG_MAX;
	time = 5.0f;
	debugId = 33;
	tag = Function::GetClassNameC<BossRock>();

	preDraw = false;
	preTransform = Transform();
	preModel = Load::LoadModel(Load::MODEL_PATH + "BossRockPre",ID::BOSS_PRE_MODEL);
	
	
}

BossRock::~BossRock()
{
	
}

void BossRock::Update()
{
	BossRockBase::Update();
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

	info.oneColl = false;
	info.tag = CollsionInformation::JUST_AVOID;
	if (justAvoidCollider == nullptr) {
		justAvoidCollider = obj->Component()->AddComponent<SphereCollider>();
		justAvoidCollider->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(450.0f, 1.0f, 1.0f)), "justAvoid_rock");
	}

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

void BossRock::SetPreInfo(const VECTOR3& _pos)
{
	preTransform = Transform(_pos,VZero,VECTOR3(5.0f,0.1f,5.0f));
	preDraw = true;
}

void BossRock::Ground()
{
	BossRockBase::Ground();
	preDraw = false;
	obj->Component()->RemoveComponentWithTagIsCollsion<RayCollider>("uiColl");
}
