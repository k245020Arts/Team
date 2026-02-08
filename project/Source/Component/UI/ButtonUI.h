#pragma once
#include "../Component.h"
#include "UIBase.h"

class InputManager;
class ControllerInputManager;
class Color;

class ButtonUI : public UIBase
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
	/// <summary>
	/// UIのアクティブモードを有効にする
	/// </summary>
	void ButtonActiveStart() { buttonActive = true; }
	/// <summary>
	/// UIのアクティブモードを無効にする
	/// </summary>
	void ButtonActiveFinish() { buttonActive = false; }

	/// <summary>
	/// UIの強調表現の開始
	/// </summary>
	void ColorGradeMode() { gradeMode = true; }
	/// <summary>
	/// UIの強調表現の終了
	/// </summary>
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
