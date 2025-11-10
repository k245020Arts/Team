#include "sphereCollider.h"
#include "Debug.h"

SphereCollider::SphereCollider()
{
	debugId = 3;
	tag = Function::GetClassNameC<SphereCollider>();
	SetTag(tag);
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Update()
{

}

void SphereCollider::Draw()
{
	if (Debug::SphereColliderDraw()) {
		DrawSphere3D(collTransform->WorldTransform().position, collTransform->scale.x, 10, 0xff0000, 0xff0000, false);
	}
}

void SphereCollider::Start()
{
	
}
