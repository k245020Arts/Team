#pragma once
#include "../../Library/GameObject.h"
#include "../GameControler/GameControler.h"


class UIManager;
class TextRenderer;

class PauseScreen : public GameObject
{
public:
	PauseScreen();
	~PauseScreen();

	void Update()override;
	void Draw()override;

	void PauseStart();
	bool PauseFinish();

	bool IsSelect() { return selectMenu; }
	

	enum PauseMenuItem
	{
		Back = 0,
		Guide,
		Title,

		Max
	};

	enum PauseResult
	{
		None = -1,
		Resume,
		OpenGuide,
		ToTitle
	};

	PauseResult GetResult() const { return result; }

	void PauseButtonGameBack();

private:
	bool pause;
	float timeRate;
	bool selectMenu;

	int currentIndex;

	PauseResult result;

	UIManager* UiManager;

	PauseMenuItem pauseItem;

	TextRenderer* pauseText;
	TextRenderer* backText;
	TextRenderer* titleText;
	TextRenderer* guideText;
	TextRenderer* selectText;
	TextRenderer* cursorText;

	std::vector<TextRenderer*> menuTexts;

	void UpdateButtonState();

	float animationTime;

	int screen;
	float selectDelayTime;
	bool delayCountStart;
	float baseSinRate;
	float inputDelayTime;

	std::vector<VECTOR3> cursorPos;
};