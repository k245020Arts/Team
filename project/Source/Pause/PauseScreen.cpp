#include "PauseScreen.h"
#include "../Component/UI/UIManager/UIManager.h"
#include "../Common/InputManager/InputManager.h"
#include "../Common/InputManager/PadInput.h"

PauseScreen::PauseScreen()
{
	pause = false;
	timeRate = 0.0f;
	UiManager = FindGameObject<UIManager>();

	pauseItem = Back;
	SetDrawOrder(-5000);
}

PauseScreen::~PauseScreen()
{
}

void PauseScreen::Update()
{
	if (!pause) {
		return;
	}
    if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.y >= 0.5f)
    {
        pauseItem = Back;
    }

    if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.y <= -0.5f)
    {
        pauseItem = Title;
    }

	if (InputManager::GetInstance()->KeyInputDown("PauseSelect"))
	{
		selectMenu = true;
	}
}

void PauseScreen::Draw()
{
	if (!pause) {
		return;
	}

	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(550, 250, "PAUSE", GetColor(255, 255, 255));

	int resumeColor =(pauseItem == PauseMenuItem::Back) ? GetColor(255, 255, 0): GetColor(255, 255, 255);

	int titleColor =(pauseItem == PauseMenuItem::Title) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);

	DrawString(560, 350, "Continue", resumeColor);
	DrawString(560, 420, "Title", titleColor);
}

void PauseScreen::PauseStart()
{
	pause = true;
	/*timeRate = Time::GetTimeRate();
	Time::ChangeDeltaRate(0.0f);*/
	UiManager->SetUIDraw(false);
	selectMenu = false; 
}

bool PauseScreen::PauseFinish()
{
	pause = false;
	/*Time::ChangeDeltaRate(timeRate);*/
	UiManager->SetUIDraw(true);

	if (pauseItem == Back) {
		return true;
	}
	else {
		return false;
	}
	selectMenu = false;
}
