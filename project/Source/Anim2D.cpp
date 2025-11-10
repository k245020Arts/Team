#include "Anim2D.h"

Anim2D::Anim2D()
{
	debugId = 20;
	tag = Function::GetClassNameC<Anim2D>();
}

Anim2D::~Anim2D()
{
}

void Anim2D::Update()
{
	if (!playing) {
		return;
	}
	animCounter += Time::DeltaTime() * animSpeed;
}

void Anim2D::AnimSetting(float _speed, int num)
{
	animSpeed = _speed;
	graphNum = num;
	playing = true;
}
