#pragma once
#include "../../Library/GameObject.h"

class TextRenderer;

class GuideWindow : public GameObject
{
public:
	GuideWindow();
	~GuideWindow();

	void Update()override;
	void Draw()override;

	void GuideWindowDraw(bool _menu);
	void GuideWindowFinish();
	/// <summary>
	/// ボタンが押されたかどうかの取得
	/// </summary>
	/// <returns>trueならボタンを押している</returns>
	bool GetPush() { return buttonPut; }
	/// <summary>
	///	ゲーム状態に遷移するかの設定
	/// </summary>
	/// <returns>trueならゲーム状態へ移行</returns>
	bool GetGameBack() { return gameBack; }

	bool GetActive() { return  active; }

private:

	bool menu;

	bool gameBack;

	TextRenderer* gameBackText;
	TextRenderer* pauseBackText;

	int guideWindowHandle;

	bool active;

	int aButtonImage;
	int bButtonImage;
	bool buttonPut;
};