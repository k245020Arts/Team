#include "light.h"

Light::Light()
{
	SetLightEnable(true);
	hLight = CreateDirLightHandle(VECTOR3(-90.0f * DegToRad, 10.0f * DegToRad, -90.0f * DegToRad));
}

Light::~Light()
{
	DeleteLightHandle(hLight);
}

void Light::Update()
{
	//dir += VECTOR3(Time::DeltaTime(), Time::DeltaTime(), Time::DeltaTime());
	//hLight = ChangeLightTypeDir(dir);
}

void Light::Draw()
{
}
