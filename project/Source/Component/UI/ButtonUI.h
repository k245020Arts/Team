#pragma once
#include "../Component.h"

class InputManager;
class ControllerInputManager;
class Color;

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
	void ButtonActiveStart() { buttonActive = true; }
	void ButtonActiveFinish() { buttonActive = false; }

	void ColorGradeMode() { gradeMode = true; }
	void ColorGradeModeFinish() { gradeMode = false; }

private:
	ButtonType type;
	int buttonImage;
	ControllerInputManager* control;
	float scale;
	bool push;
	bool buttonActive;
	bool gradeMode;
	float r, g, b;
	int backImage;
};
