#include "ResultUi.h"
#include "../Component/Transform/Transform.h"
#include "../Common/Easing.h"
#include "../Screen.h"

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

	flashTime = 0.0f;
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
	normalExrate = Easing::EaseInBack(1.0f, 0.0f, scaleCount);
	addDrawValue = Easing::EaseIn(0, 30, addCount);
	addExrate += 1.0f;
}

void ResultUi::Draw()
{
	Transform* transform = obj->GetTransform();
	if (win) {
		const int LOOP_NUM = 10;
		double size = (double)addExrate / LOOP_NUM;
		SetDrawBlendMode(DX_BLENDMODE_ADD, addDrawValue);
		for (int i = 0; i < LOOP_NUM; i++) {
			double t = (double)i / LOOP_NUM;
			double s = size * (1.0 + t * 0.3);
			double rot = t * 360.0 * DegToRad;
			DrawRotaGraph((int)transform->position.x, (int)transform->position.y, s, rot, hImage, true);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		double scale = (double)normalExrate;
		if (scale > 1.0) {
			scale = 1.0;
		}
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y, scale, 0.0, hImage, true);


		if (addCount > flashTime)
		{
			int flashAlpha = (int)((1.0f - addCount / flashTime) * 255.0f);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, flashAlpha);
			DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, 0xffffff, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	else {
		float t = min(1.0 - addCount, 1.0f);

		// 画面暗転
		int fade = (int)(t * 180);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);
		DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 少し上から落とす
		float offsetY = (1.0f - t) * -50.0f;

		// 少し暗めに描画
		SetDrawBright(180, 180, 180);
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y + offsetY, normalExrate, 0.0, hImage, true);
		SetDrawBright(255, 255, 255);
	}
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
	flashTime = 0.8f;
}