#pragma once
#include "../Component.h"

class Color;
class MeshRenderer;

class Shadow : public Component
{
public:
	Shadow();
	~Shadow();

	void Update()override;
	void Draw()override;

	void Start()override;

	void ChangeScale(VECTOR3 _dist, VECTOR3 _groundPos);

private:
	int model;
	VECTOR3 baseScale;
	const float MAX_DIST = 10000.0f;
	VECTOR3 groundPos;
	VECTOR3 dist;
};
