#pragma once
#include "../Component.h"

class InputManager;
class ControllerInputManager;

class ButtonUI : public Component
{
public:
	enum ButtonType
	{
		NONE = -1,
		A_BUTTON = 0,
		B_BUTTON,
		X_BUTTON,
		Y_BUTTON,

		BUTTON_MAX,
	};
	ButtonUI();
	~ButtonUI();

	void Update()override;
	void Draw()override;

	void Start(ButtonType _buttonType,int _handle);

private:
	ButtonType type;
	int buttonImage;
	ControllerInputManager* control;
	float scale;
};
