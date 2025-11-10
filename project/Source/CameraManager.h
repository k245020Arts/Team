#pragma once
#include "../Library/GameObject.h"

class Object3D;

class CameraManager : public GameObject
{
public:
	CameraManager();
	~CameraManager();
	void Update() override;
	void Draw() override;

	void CreateCamera();
	void ParentObj(std::string _str);
	Object3D* GetCamera() { return mainCamera; }

private:
	Object3D* mainCamera;
};
