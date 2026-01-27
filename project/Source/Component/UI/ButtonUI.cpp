#include "ButtonUI.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Component/Transform/Transform.h"

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
		obj->GetTransform()->scale = 0.5f;
	}
	else {
		obj->GetTransform()->scale = 1.0f;
	}
}

void ButtonUI::Draw()
{
	Transform* transform = obj->GetTransform();
	DrawRotaGraph(transform->position.x, transform->position.y, transform->scale.x, 0.0f, buttonImage, true);
}

void ButtonUI::Start(ButtonType _buttonType, int _handle)
{
	buttonImage = _handle;
	type = _buttonType;
	control = FindGameObject<ControllerInputManager>();
}
