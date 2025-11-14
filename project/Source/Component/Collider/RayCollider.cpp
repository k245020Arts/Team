#include "rayCollider.h"
#include "../Transform/Transform.h"
#include "../../Common/Debug/Debug.h"

RayCollider::RayCollider()
{
	debugId = 20;
	tag = Function::GetClassNameC<RayCollider>();
	SetTag(tag);
}

RayCollider::~RayCollider()
{
	delete transform2;
}

void RayCollider::Update()
{
}

void RayCollider::Draw()
{
	if (Debug::RayColliderDraw()) {
		DrawLine3D(collTransform->WorldTransform().position, transform2->WorldTransform().position, 0xff0000);
	}
}

void RayCollider::Start()
{
	
}

void RayCollider::RaySet(CollsionInfo _info, Transform _transform, Transform _transform2)
{
	ColliderBase::CollsionAdd(_info, _transform);

	transform2 = new Transform( _transform2);
	transform2->SetParent(_info.parentTransfrom);
}
