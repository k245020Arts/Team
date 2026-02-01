#include "Fead.h"
#include "../../Screen.h"

Fead::Fead()
{
	feedTime = 0.0f;
	feedCountMax = 0.0f;

	color = 0;;
	alpha = 0;;
}

Fead::~Fead()
{
}

void Fead::Update()
{
	
}

void Fead::Draw()
{
	if (feedTime < 0.0f) {
		return;
	}
	feedTime -= Time::DeltaTimeRate();
	if (feedTime <= 0.0f) {
		feedTime = 0.0f;
	}
	float rate = 0.0f;

	if (feadIn) {
		rate = feedTime / feedCountMax;
	}
	else {
		rate = 1 - (feedTime / feedCountMax);
	}
	alpha = easingFunc(0, 255, rate);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBoxAA(0, 0, Screen::WIDTH, Screen::HEIGHT, color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Fead::FeadIn(float _time, int _color, std::function<int(int, int, float)> _func)
{
	if (IsFead()) {
		return;
	}
	feedCountMax = _time;
	feedTime = _time;
	alpha = 255;
	color = _color;
	easingFunc = _func;
	feadIn = true;
}

void Fead::FeadOut(float _time, int _color, std::function<int(int, int, float)> _func)
{
	if (IsFead()) {
		return;
	}
	feedCountMax = _time;
	feedTime = _time;
	alpha = 255;
	color = _color;
	easingFunc = _func;
	feadIn = false;
}
