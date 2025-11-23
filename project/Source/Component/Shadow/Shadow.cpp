#include "Shadow.h"
#include "../../Common/ID/ID.h"
#include "../../Common/LoadManager.h"
#include "../MeshRenderer/MeshRenderer.h"
#include "../Color/Color.h"
#include "../ComponentManager.h"
#include "../../Common/Easing.h"
#include "../Transform/Transform.h"

Shadow::Shadow()
{
	model = -1;
	debugId = 24;
	tag = Function::GetClassNameC<Shadow>();
}

Shadow::~Shadow()
{
}

void Shadow::Update()
{
	GetBaseObject()->GetTransform()->position.y = groundPos.y;
}

void Shadow::Draw()
{
}

void Shadow::Start()
{
	model = Load::LoadModel(Load::MODEL_PATH + "shadow", ID::SHADOW);
	MeshRenderer* mesh = GetBaseObject()->Component()->AddComponent<MeshRenderer>();
	mesh->ModelHandle(model);
	//mesh->DrawLocalPosition();
	GetBaseObject()->Component()->GetComponent<Color>()->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
	baseScale = GetBaseObject()->GetTransform()->scale;
}

void Shadow::ChangeScale(VECTOR3 _dist, VECTOR3 _groundPos)
{
	float base = baseScale.x;
	float d = _dist.y;
	if (d > MAX_DIST) {
		d = MAX_DIST;
	}
	float s = Easing::Lerp(base, 0.2f, d / MAX_DIST);
	GetBaseObject()->GetTransform()->scale.x = s;
	GetBaseObject()->GetTransform()->scale.z = s;
	groundPos = _groundPos;
	dist = _dist;
}
