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

	void ChangeScale();

private:
	int model;
	VECTOR3 scale;
};
