#include "ModelCollider.h"
#include "Debug.h"

ModelCollider::ModelCollider()
{
	debugId = 2;
	tag = Function::GetClassNameC<ModelCollider>();
	SetTag(tag);
}

ModelCollider::~ModelCollider()
{
	MV1TerminateCollInfo(modelHandle, -1);
	MV1DeleteModel(modelHandle);
}

void ModelCollider::Update()
{
	MV1SetMatrix(modelHandle, collTransform->WorldTransform().GetMatrix());

	int a = MV1RefreshCollInfo(modelHandle, -1);
}

void ModelCollider::Draw()
{
	if (!Debug::ModelColliderDraw()) {
		return;
	}
	MV1DrawModel(modelHandle);
}

void ModelCollider::ModelColliderSet(CollsionInfo _info, Transform _transform, int _modelHandle)
{
	ColliderBase::CollsionAdd(_info, _transform);

	modelHandle = _modelHandle;
	int i = MV1SetupCollInfo(modelHandle, -1, 8, 8, 8);
}