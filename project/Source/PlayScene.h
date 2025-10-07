#pragma once
#include "../Library/SceneBase.h"
#include <vector>

/// <summary>
/// ゲームプレイのシーンを制御する
/// </summary>

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	static int count;
};

