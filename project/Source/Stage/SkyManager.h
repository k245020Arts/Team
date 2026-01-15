#pragma once
#include "../../Library/GameObject.h"

class Object3D;

static const VECTOR3 CUT_SCENE_POS = VECTOR3(1000000.0f, 0.0f, 1000000.0f);

class SkyManager : public GameObject
{
public:
	SkyManager();
	~SkyManager();
	void Update()override;
	void Draw()override;

	void CreateSky();

private:
	Object3D* sky;
	Object3D* sky2;
};