#include "cameraManager.h"
#include "../Component/Object/Object3D.h"
#include "../Component/Transform/Transform.h"
#include "camera.h"
#include "../Enemy/EnemyManager.h"
#include "../Component/Light/ComponentLight.h"
#include "../Component/ComponentManager.h"
#include "../Component/Collider/ModelCollider.h"
#include "../Common/LoadManager.h"
#include "../Component/Collider/SphereCollider.h"

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::Update()
{
}

void CameraManager::Draw()
{
}

void CameraManager::CreateCamera()
{
	mainCamera = new Object3D();
	mainCamera->Init(VECTOR3(0,0,-1500), VZero, VECTOR3(1, 1, 1), "CAMERA_OBJ");

	Camera* camera = mainCamera->Component()->AddComponent<Camera>();
	camera->Start(nullptr);

	SphereCollider* c = mainCamera->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.oneColl = false;
	info.parentTransfrom = camera->GetCameraTransform();
	info.shape = CollsionInformation::SPHERE;
	info.tag = CollsionInformation::C_FLOOR;
	c->CollsionAdd(info, Transform(VZero, VZero, VOne * VECTOR3(100,0,0)));

	mainCamera->SetDrawOrder(-1);

}

void CameraManager::ParentObj(std::string _str)
{
	Object3D* obj = FindGameObjectWithTag<Object3D>(_str);
	mainCamera->Component()->GetComponent<Camera>()->PlayerSet(obj);

	
}
