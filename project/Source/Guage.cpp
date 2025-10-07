#include "Guage.h"
#include <cmath>
#include <iostream>
#include <algorithm>

Guage::Guage()
{
	edge = nullptr;
	guage = nullptr;
	maxhp = 0.0f;
	hp = nullptr;
	debugId = 19;
	tag = Function::GetClassNameC<Guage>();
	displayHp = 0.0f;
}

Guage::~Guage()
{

}

void Guage::Update()
{
	if (guage != nullptr) {

		float speed = 3.0f;
		displayHp += (*hp - displayHp) * speed * Time::DeltaTimeRate();

		displayHp =  std::clamp(displayHp, 0.0f, maxhp);

		// Š„‡‚©‚ç•`‰æˆÊ’u‚ð’²®
		float amount = displayHp / maxhp;

		//float amout = *hp / maxhp;
		guage->SetStartPos(VECTOR2I(static_cast<int>((1.0f - amount) * guage->GetImageSize().x), 0));
	}
}

void Guage::Draw()
{
}

void Guage::EdgeDrawReady(int _image, MeshRenderer2D::GraphMode _mode)
{
	edge = obj->Component()->AddComponent<MeshRenderer2D>();
	edge->TextureHandle(_image, _mode);
}