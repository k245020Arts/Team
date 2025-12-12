#include "Guage.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include "../../Camera/CameraManager.h"

Guage::Guage()
{
	edge = nullptr;
	guage = nullptr;
	maxhp = 0.0f;
	hp = nullptr;
	debugId = 19;
	tag = Function::GetClassNameC<Guage>();
	displayHp = 0.0f;
	changeColorCounter = 0.0f;
	screenMode = false;
}

Guage::~Guage()
{
	edge = nullptr;
	guage = nullptr;
	hp = nullptr;
}

void Guage::Update()
{
	if (screenMode) {
		VECTOR3 worldPos(
			obj->GetParent()->GetTransform()->position.x,
			obj->GetParent()->GetTransform()->position.y,
			obj->GetParent()->GetTransform()->position.z
		);

		VECTOR headPos = VAdd(worldPos,plus);

		VECTOR3 screenPos = VECTOR3(0,0,0);
		screenPos = ConvWorldPosToScreenPos(headPos);
		obj->GetTransform()->position = screenPos;
		edge->SetPosition(obj->GetTransform()->position);
	}
	if (guage != nullptr) {

		float speed = 3.0f;
		displayHp += (*hp - displayHp) * speed * Time::DeltaTimeRate();

		displayHp =  std::clamp(displayHp, 0.0f, maxhp);

		// Š„‡‚©‚ç•`‰æˆÊ’u‚ð’²®
		float amount = displayHp / maxhp;

		//float amout = *hp / maxhp;

		guage->SetDrawImageSize(VECTOR2I(static_cast<int>((amount) * guage->GetImageSize().x), guage->GetImageSize().y));
	}
}

void Guage::Draw()
{
}

void Guage::EdgeDrawReady(int _image, MeshRenderer2D::GraphMode _mode, Transform _transfrom)
{
	edge = obj->Component()->AddComponent<MeshRenderer2D>();
	edge->TextureHandle(_image, _mode);
	edge->SetTransform(_transfrom);
}

void Guage::WorldToScreenMode(bool _mode, VECTOR3 _plusPos)
{
	screenMode = _mode;
	plus = _plusPos;
}
