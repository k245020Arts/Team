#pragma once
#include "Component.h"

class Transform;
class EffectManager;

class Wall : public Component
{
public:
	Wall();
	~Wall();

	void Update()override;
	void Draw()override;

	void ModelSet(int _modelHandle);

	void ImguiDraw()override;

private:
	int model;
	Transform* pTransform;
	Transform* transform;
	EffectManager* effect;
	int count;
};