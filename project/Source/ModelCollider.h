#pragma once
#include "colliderBase.h"
#include "transform.h"

class ModelCollider : public ColliderBase
{
public:
	ModelCollider();
	~ModelCollider();

	void Update()override;
	//void Draw()override;

	void ModelColliderSet(CollsionInfo _info, Transform _transform , int _modelHandle);

	int GetModel() { return modelHandle; }

private:
	int modelHandle; //“–‚½‚è”»’è‚ÉŽg‚¤ƒ‚ƒfƒ‹
};