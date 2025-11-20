#pragma once
#include "../../Library/GameObject.h"

class Object3D;

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
};