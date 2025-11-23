#include "Shadow.h"
#include "../../Common/ID/ID.h"
#include "../../Common/LoadManager.h"
#include "../MeshRenderer/MeshRenderer.h"
#include "../Color/Color.h"
#include "../ComponentManager.h"

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
}

void Shadow::Draw()
{
}

void Shadow::Start()
{
	model = Load::LoadModel(Load::MODEL_PATH + "shadow", ID::SHADOW);
	MeshRenderer* mesh = GetBaseObject()->Component()->AddComponent<MeshRenderer>();
	mesh->ModelHandle(model);
	GetBaseObject()->Component()->GetComponent<Color>()->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
}

void Shadow::ChangeScale()
{
	float base = 0.0f;
	
}
