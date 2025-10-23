#include "skyManager.h"
#include "Object3D.h"
#include "MeshRenderer.h"
#include "ComponentManager.h"
#include "LoadManager.h"
#include "transform.h"

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

	MeshRenderer* mesh = sky->Component()->AddComponent<MeshRenderer>();
	mesh->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "sky/sky_2", ID::SKY));

}
