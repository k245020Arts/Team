#pragma once
#include "../Library/GameObject.h"


class InputManager;
class SoundManager;

class TitlePlayer;

class TitleControl : public GameObject
{
public:
	TitleControl();
	~TitleControl();

	void Update()override;
	void Draw()override;

	float GetNowProgress();
	void SetNowProgress(float nowProgress);
	float progress;

	void StageSelect();

private:
	int hImage;
	int keyImage;
	int titleImage;
	float firstCounter;
	float exrate;
	float pushCounter;
	float moveButton;

	TitlePlayer* player;
	int stageID;
	float selectCounter;
};
