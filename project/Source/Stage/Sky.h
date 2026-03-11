#pragma once
#include "../../Library/GameObject.h"

class Object3D;

static const VECTOR3 CUT_SCENE_POS = VECTOR3(1000000.0f, 0.0f, 1000000.0f);

class Sky : public GameObject
{
public:
	Sky();
	~Sky();
	void Update()override;
	void Draw()override;
	/// <summary>
	/// SKYSphereÇÃê∂ê¨
	/// </summary>
	void CreateSky();

private:
	Object3D* sky;
	Object3D* sky2;
};