#pragma once
#include "../Library/SceneBase.h"
#include <vector>

class InputManager;

/// <summary>
/// ゲームプレイのシーンを制御する
/// </summary>

class PlayScene : public SceneBase
{
public:
	enum Mode
	{
		NORMAL,
		CAMERA_EDITOR,
	};

	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

	Mode GetMode() { return mode; }
	void SetMode(Mode _mode) { mode = _mode; }
	static int count;
private:
	InputManager* input;

	
	Mode mode;
};

