#include "CharaBase.h"
#include "sphereCollider.h"

CharaBase::CharaBase()
{
	lastTarget = false;
}

CharaBase::~CharaBase()
{
}

void CharaBase::Update()
{
}

void CharaBase::Draw()
{
}

void CharaBase::DeleteColliderComponent()
{
	switch (collShape)
	{
	case CollsionInformation::SPHERE:
		obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>(collName);
		break;
	case CollsionInformation::MODEL:
		break;
	case CollsionInformation::RAY:
		break;
	case CollsionInformation::AABB:
		break;
	case CollsionInformation::SHAPE_MAX:
		break;
	default:
		break;
	}
}
