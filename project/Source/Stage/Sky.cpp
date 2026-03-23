#include "Sky.h"
#include "../Component/Object/Object3D.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/ComponentManager.h"
#include "../Common/ResourceLoader.h"
#include "../Component/Transform/Transform.h"

Sky::Sky()
{
	sky = nullptr;
}

Sky::~Sky()
{
}

void Sky::Update()
{
	//‹ó‚ð‰ñ“]
	sky->GetTransform()->rotation.y += 0.01f * DegToRad;
}

void Sky::Draw()
{
}

void Sky::CreateSky()
{
	sky =  new Object3D();

	sky->Init(VECTOR3(0, -4000, 0), VECTOR3(0, 0, 0), VECTOR3(2000.0f,2000.0f,2000.0f), "sky");
	sky->SetDrawOrder(10);

	MeshRenderer* mesh = sky->Component()->AddComponent<MeshRenderer>();
	mesh->ModelHandle(ResourceLoad::LoadModel(ResourceLoad::MODEL_PATH + "sky/sky_2", ID::SKY));

	sky2 = new Object3D();

	sky2->Init(VECTOR3(CUT_SCENE_POS), VECTOR3(0, 0, 0), VECTOR3(100.0f, 100.0f, 100.0f), "sky");
	sky2->SetDrawOrder(10);

	MeshRenderer* mesh2 = sky2->Component()->AddComponent<MeshRenderer>();
	mesh2->ModelHandle(ResourceLoad::LoadModel(ResourceLoad::MODEL_PATH + "sky/sky_2", ID::SKY));

}
