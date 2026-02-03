#include "ButtonUI.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/Color/Color.h"
#include "../ComponentManager.h"

namespace {
	int buttonNum[ButtonUI::ButtonType::BUTTON_MAX] = {
		XINPUT_BUTTON_A,XINPUT_BUTTON_B,
		XINPUT_BUTTON_X,XINPUT_BUTTON_Y 
	};
}

ButtonUI::ButtonUI()
{
	type = NONE;
	buttonImage = -1;
	debugId = 31;
	tag = Function::GetClassNameC<ButtonUI>();
	gradeMode = false;
	buttonActive = false;
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;

}

ButtonUI::~ButtonUI()
{
}

void ButtonUI::Update()
{
	if (type == NONE) {
		my_error_assert("ボタンの種類がセットされていません。");
		return;
	}
	if (control->GetIsButtonPushingNow(buttonNum[type])) {
		//obj->GetTransform()->scale = 0.5f;
		push = true;
	}
	else {
		//obj->GetTransform()->scale = 1.0f;
		push = false;
	}
	if (gradeMode) {
		r += Time::DeltaTimeRate() * 255.0f;
		g += Time::DeltaTimeRate() * 255.0f;
		b += Time::DeltaTimeRate() * 255.0f;
		if (r >= 255) {
			r = 0;
		}
		if (g >= 255) {
			g = 0;
		}
		if (b >= 255) {
			b = 0;
		}
	}
}

void ButtonUI::Draw()
{
	Transform* transform = obj->GetTransform();
	if (push || !buttonActive) {
		SetDrawBright(50, 50, 50);
	}
	else {
		SetDrawBright(200, 200, 200);
	}
	if (gradeMode) {
		SetDrawBright(r, g, b);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 60);
		float add = transform->scale.x / 8;
		for (int i = 0; i < 8; i++) {
			DrawRotaGraph((int)transform->position.x, (int)transform->position.y, (double)add * i, 0.0, backImage, true);	
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y, transform->scale.x, 0.0, buttonImage, true);
	}
	else {
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y, (double)transform->scale.x, 0.0, backImage, true);
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y, (double)transform->scale.x, 0.0, buttonImage, true);
	}

	
	SetDrawBright(255, 255, 255);
}

void ButtonUI::Start(ButtonType _buttonType, int _handle)
{
	buttonImage = _handle;
	type = _buttonType;
	control = FindGameObject<ControllerInputManager>();
	obj->GetTransform()->scale = 0.8f;
	buttonActive = true;
	backImage = LoadGraph("data/image/UICicle.png");
}
