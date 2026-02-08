#include "ShockWave.h"
#include "../../Transform/Transform.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../Collider/DountCollider.h"
#include "../../ComponentManager.h"

ShockWave::ShockWave()
{
	effect		= FindGameObject<EffectManager>();
	collider	= nullptr;
	waveSpeed	= 0.0f;
	debugId		= 24;
	tag			= Function::GetClassNameC<ShockWave>();
}

ShockWave::~ShockWave()
{
}

void ShockWave::Update()
{
	collider->GetTransform()->scale.x += waveSpeed;
	collider->AddOutRadius(waveSpeed);
}

void ShockWave::Draw()
{
}

void ShockWave::CreateWave(CollsionInformation::Tag _tag, const Transform& _transform, float _startOutRange, float _speed)
{
	
	CollsionInfo info;
	info.parentTransfrom	= GetBaseObject()->GetTransform();
	info.shape				= CollsionInformation::DONUT;
	info.oneColl			= true;
	info.tag				= _tag;
	collider				= obj->Component()->AddComponent<DountCollider>();
	collider->DountSet(info, _transform, _startOutRange);
	waveSpeed				= _speed;
}
