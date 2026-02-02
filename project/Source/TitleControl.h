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

	float progress;
private:
	int hImage;
	InputManager* input;
	int keyImage;
	int titleImage;
	SoundManager* sound;
	float firstCounter;
	float exrate;
	float pushCounter;
	float moveButton;

	TitlePlayer* player;
};
