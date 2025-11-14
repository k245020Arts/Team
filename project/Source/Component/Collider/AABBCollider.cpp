#include "AABBCollider.h"

AABBCollider::AABBCollider()
{
	debugId = 1;
	tag = Function::GetClassNameC<AABBCollider>();
	SetTag(tag);
}

AABBCollider::~AABBCollider()
{
}

void AABBCollider::Update()
{
}

void AABBCollider::Draw()
{
}
