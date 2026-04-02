#include "Shadow.h"
#include "../../Common/ID/ID.h"
#include "../../Common/ResourceLoader.h"
#include "../MeshRenderer/MeshRenderer.h"
#include "../Color/Color.h"
#include "../ComponentManager.h"
#include "../../Common/Easing.h"
#include "../Transform/Transform.h"

Shadow::Shadow()
{
	model		= -1;
	debugId		= 24;
	tag			= Function::GetClassNameC<Shadow>();
	height = -1;
	obj = nullptr;
}

Shadow::~Shadow()
{
}

void Shadow::Update()
{
	obj->GetTransform()->position = obj->GetParent()->GetTransform()->position;
	obj->GetTransform()->position.y = groundPos.y;
}

void Shadow::Draw()
{

}

void Shadow::Start(float _height)  
{  
	obj = GetBaseObject();
    model = ResourceLoad::LoadModel(ResourceLoad::MODEL_PATH + "shadow1", ID::SHADOW);
    MeshRenderer* mesh = GetBaseObject()->Component()->AddComponent<MeshRenderer>();  
    mesh->ModelHandle(model);  
    mesh->DrawLocalPosition();  
    baseScale = obj->GetTransform()->scale;
	height = _height;
}

void Shadow::ChangeScale(const VECTOR3& _dist, const VECTOR3& _groundPos)
{
	float base = baseScale.x;
	float d = _dist.y;
	if (d > MAX_DIST) {
		d = MAX_DIST;
	}
	float s = Easing::EaseOut(base, 0.2f, d / MAX_DIST);
	obj->GetTransform()->scale.x = s;
	obj->GetTransform()->scale.z = s;
	groundPos = _groundPos + VECTOR3(0, height, 0); //âeÇ™ÇµÇ¡Ç©ÇËï`âÊÇ≥ÇÍÇÈÇÊÇ§Ç…ínñ ÇÊÇËè≠ÇµçÇÇ≠ê›íË
	dist = _dist;
}
