#include "ResultUi.h"
#include "../Component/Transform/Transform.h"
#include "../Common/Easing.h"

ResultUi::ResultUi()
{
	hImage = -1;
	win = false;
	normalExrate = 0.0f;
	addExrate = 0.0f;
	start = false;
	addCount = 0.0f;
	addDrawValue = 0;
	scaleCount = 0.0f;

	debugId = 35;
	tag = Function::GetClassNameC<ResultUi>();
	Load::LoadImageGraph(Load::IMAGE_PATH + "Win", ID::WIN);
	Load::LoadImageGraph(Load::IMAGE_PATH + "Lose", ID::LOSE);
}

ResultUi::~ResultUi()
{
}

void ResultUi::Update()
{
	if (!start) {
		return;
	}
	addCount = max(addCount - Time::DeltaTimeRate() * 1.0f, 0.0f);
	scaleCount = max(scaleCount - Time::DeltaTimeRate() * 1.5f, 0.0f);
	normalExrate = Easing::EaseInExpo(1.0f, 0.0f, scaleCount);
	addDrawValue = Easing::EaseIn(0, 30, addCount);
	addExrate += 1.0f;
}

void ResultUi::Draw()
{
	Transform* transform = obj->GetTransform();

	const int LOOP_NUM = 10;
	double size = (double)addExrate / LOOP_NUM;
	SetDrawBlendMode(DX_BLENDMODE_ADD, addDrawValue);
	for (int i = 0; i < 10; i++) {
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y, size, 0.0, hImage, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	double scale = (double)normalExrate;
	if (scale > 1.0) {
		scale = 1.0;
	}
	DrawRotaGraph((int)transform->position.x, (int)transform->position.y,scale,0.0, hImage, true);

}

void ResultUi::ResultStart(bool _win)
{
	if (start) {
		return;
	}
	if (_win) {
		hImage = Load::GetHandle(ID::WIN);
	}
	else {
		hImage = Load::GetHandle(ID::LOSE);
	}
	
	win = _win;
	start = true;
	addCount = 1.0f;
	addDrawValue = 0;
	scaleCount = 1.0f;
}