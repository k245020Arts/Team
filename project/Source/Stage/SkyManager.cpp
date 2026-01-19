#include "skyManager.h"
#include "../Component/Object/Object3D.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/ComponentManager.h"
#include "../Common/LoadManager.h"
#include "../Component/Transform/Transform.h"

SkyManager::SkyManager()
{
	sky = nullptr;
}

SkyManager::~SkyManager()
{
}

void SkyManager::Update()
{
	//‹ó‚ð‰ñ“]
	sky->GetTransform()->rotation.y += 0.01f * DegToRad;
}

void SkyManager::Draw()
{
}

void SkyManager::CreateSky()
{
	sky =  new Object3D();

	sky->Init(VECTOR3(0, -4000, 0), VECTOR3(0, 0, 0), VECTOR3(2000.0f,2000.0f,2000.0f), "sky");
	sky->SetDrawOrder(10);

	MeshRenderer* mesh = sky->Component()->AddComponent<MeshRenderer>();
	mesh->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "sky/sky_2", ID::SKY));

	sky2 = new Object3D();

	sky2->Init(VECTOR3(CUT_SCENE_POS), VECTOR3(0, 0, 0), VECTOR3(100.0f, 100.0f, 100.0f), "sky");
	sky2->SetDrawOrder(10);

	MeshRenderer* mesh2 = sky2->Component()->AddComponent<MeshRenderer>();
	mesh2->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "sky/sky_2", ID::SKY));

}
