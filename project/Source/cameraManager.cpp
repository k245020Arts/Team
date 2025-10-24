#include "cameraManager.h"
#include "Object3D.h"
#include "transform.h"
#include "camera.h"
#include "EnemyManager.h"
#include "ComponentLight.h"
#include "ComponentManager.h"
#include "ModelCollider.h"
#include "LoadManager.h"
#include "sphereCollider.h"

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
	camera->Start(*(FindGameObject<EnemyManager>()->GetEnemy().begin()));

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
