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
#include "../Component/Collider/RayCollider.h"
#include "../../Source/Component/Physics/Physics.h"
#include "../State/StateManager.h"
#include "../TitleCamera.h"

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
	mainCamera->Init(VECTOR3(0,3000,-6000), VZero, VECTOR3(1, 1, 1), "CAMERA_OBJ");

	mainCamera->Component()->AddComponent<StateManager>();

	Camera* camera = mainCamera->Component()->AddComponent<Camera>();
	camera->Start(nullptr);

	/*SphereCollider* c = mainCamera->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.oneColl = false;
	info.parentTransfrom = camera->GetCameraTransform();
	info.shape = CollsionInformation::SPHERE;
	info.tag = CollsionInformation::C_FLOOR;
	c->CollsionAdd(info, Transform(VZero, VZero, VOne * VECTOR3(100,0,0)));*/

	

	mainCamera->Component()->AddComponent<Physics>()->Start(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f));

	mainCamera->SetDrawOrder(-1);

}

void CameraManager::ParentObj(std::string _str)
{
	Object3D* obj = FindGameObjectWithTag<Object3D>(_str);
	mainCamera->Component()->GetComponent<Camera>()->PlayerSet(obj);

	RayCollider* c = mainCamera->Component()->AddComponent<RayCollider>();
	CollsionInfo info;
	info.oneColl			= false;
	info.parentTransfrom	= mainCamera->Component()->GetComponent<Camera>()->GetCameraTransform();
	info.shape				= CollsionInformation::RAY;
	info.tag				= CollsionInformation::C_FLOOR;
	c->RaySet(info, Transform(VECTOR3(0, -100, 0), VZero, VOne), Transform(VECTOR3(0, 100, 0), VZero, VOne));
}

void CameraManager::TitleCameraCreate()
{
	mainCamera = new Object3D();
	mainCamera->Init(VECTOR3(0, 3000, -6000), VZero, VECTOR3(1, 1, 1), "CAMERA_OBJ");

	//mainCamera->Component()->AddComponent<StateManager>();

	TitleCamera* camera = mainCamera->Component()->AddComponent<TitleCamera>();
	camera->Start(mainCamera);

	/*SphereCollider* c = mainCamera->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.oneColl = false;
	info.parentTransfrom = camera->GetCameraTransform();
	info.shape = CollsionInformation::SPHERE;
	info.tag = CollsionInformation::C_FLOOR;
	c->CollsionAdd(info, Transform(VZero, VZero, VOne * VECTOR3(100,0,0)));*/



	mainCamera->Component()->AddComponent<Physics>()->Start(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f));

	mainCamera->SetDrawOrder(-6);


}

void CameraManager::TitleParentObj(std::string _str)
{
	Object3D* obj = FindGameObjectWithTag<Object3D>(_str);
	mainCamera->Component()->GetComponent<TitleCamera>()->PlayerSet(obj);
}
