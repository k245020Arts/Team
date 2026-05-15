#include "PauseScreen.h"
#include "../Component/UI/UIManager/UIManager.h"
#include "../Common/InputManager/InputManager.h"
#include "../Common/InputManager/PadInput.h"
#include "../Common/ResourceLoader.h"
#include "../Component/UI/TextRenderer.h"
#include "../Component/ComponentManager.h"

PauseScreen::PauseScreen()
{
	pause = false;
	timeRate = 0.0f;
	UiManager = FindGameObject<UIManager>();

	pauseItem = Back;
	SetDrawOrder(-5000);
	ResourceLoad::LoadFont("MonopinJRegular", ".dft", Font_ID::PAUSE_FONT, 4);

	const int EDGE_NUM_X = 300;
	const int EDGE_NUM_Y = 200;

	Object2D* pauseObj = new Object2D();
	pauseObj->Init(VECTOR2F(550.0f + EDGE_NUM_X,250.0f + EDGE_NUM_Y),VECTOR2F(0.0f,0.0f),VECTOR2F(1.0f,1.0f),"PauseText");
	pauseText = pauseObj->Component()->AddComponent<TextRenderer>();
	pauseText->TextSetting("Pause", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);

	Object2D* back = new Object2D();
	back->Init(VECTOR2F(560.0f + EDGE_NUM_X, 350.0f + EDGE_NUM_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "BackText");
	backText = back->Component()->AddComponent<TextRenderer>();
	backText->TextSetting("Continue", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);

	Object2D* title = new Object2D();
	title->Init(VECTOR2F(560.0f + EDGE_NUM_X, 420.0f + EDGE_NUM_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "TitleText");
	titleText = title->Component()->AddComponent<TextRenderer>();
	titleText->TextSetting("Title", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);

	pauseText->SetAlpha(0.0f);
	backText->SetAlpha(0.0f);
	titleText->SetAlpha(0.0f);
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

	//DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	const int EDGE_NUM_X = 600;
	const int EDGE_NUM_Y = 300;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(EDGE_NUM_X, EDGE_NUM_Y, 780 + EDGE_NUM_X, 620 + EDGE_NUM_Y, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	/*DrawString(550, 250, "PAUSE", GetColor(255, 255, 255));*/

	(pauseItem == PauseMenuItem::Back) ? backText->SetColor(ORANGE) : backText->SetColor(WHITE);

	(pauseItem == PauseMenuItem::Title) ? titleText->SetColor(ORANGE) : titleText->SetColor(WHITE);

	/*DrawString(560, 350, "Continue", resumeColor);
	DrawString(560, 420, "Title", titleColor);*/
}

void PauseScreen::PauseStart()
{
	pause = true;
	/*timeRate = Time::GetTimeRate();
	Time::ChangeDeltaRate(0.0f);*/
	//UiManager->SetUIDraw(false);
	selectMenu = false; 

	pauseText->SetAlpha(255.0f);
	backText->SetAlpha(255.0f);
	titleText->SetAlpha(255.0f);
}

bool PauseScreen::PauseFinish()
{
	pause = false;
	/*Time::ChangeDeltaRate(timeRate);*/
	//UiManager->SetUIDraw(true);

	pauseText->SetAlpha(0.0f);
	backText->SetAlpha(0.0f);
	titleText->SetAlpha(0.0f);

	selectMenu = false;

	if (pauseItem == Back) {
		return true;
	}
	else {
		return false;
	}
}
