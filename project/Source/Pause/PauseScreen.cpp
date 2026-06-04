#include "PauseScreen.h"
#include "../Component/UI/UIManager/UIManager.h"
#include "../Common/InputManager/InputManager.h"
#include "../Common/InputManager/PadInput.h"
#include "../Common/ResourceLoader.h"
#include "../Component/UI/TextRenderer.h"
#include "../Component/ComponentManager.h"
#include "../Screen.h"
#include "../Common/Sound/SoundManager.h"

PauseScreen::PauseScreen()
{
	pause = false;
	screen = -1;
	timeRate = 0.0f;
	UiManager = FindGameObject<UIManager>();
	animationTime = 0.0f;

	pauseItem = Back;
	SetDrawOrder(-300000);
	ResourceLoad::LoadFont("MonopinJRegular", ".dft", Font_ID::PAUSE_FONT, 4);

	const int EDGE_NUM_X = 300;
	const int EDGE_NUM_Y = 50;

	Object2D* pauseObj = new Object2D();
	pauseObj->Init(VECTOR2F(440.0f + EDGE_NUM_X,150.0f + EDGE_NUM_Y),VECTOR2F(0.0f,0.0f),VECTOR2F(2.0f,2.0f),"PauseText");
	pauseText = pauseObj->Component()->AddComponent<TextRenderer>();
	pauseText->TextSetting("ポーズ画面", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	pauseObj->SetDrawOrder(-350000);

	Object2D* back = new Object2D();
	back->Init(VECTOR2F(525.0f + EDGE_NUM_X, 350.0f + EDGE_NUM_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "BackText");
	backText = back->Component()->AddComponent<TextRenderer>();
	backText->TextSetting("ゲームに戻る", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	back->SetDrawOrder(-350000);

	Object2D* title = new Object2D();
	title->Init(VECTOR2F(505.0f + EDGE_NUM_X, 500.0f + EDGE_NUM_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "TitleText");
	titleText = title->Component()->AddComponent<TextRenderer>();
	titleText->TextSetting("タイトルに戻る", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	title->SetDrawOrder(-350000);

	Object2D* select = new Object2D();
	select->Init(VECTOR2F(355.0f + EDGE_NUM_X, 700.0f + EDGE_NUM_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.5f, 1.5f), "Select");
	selectText = select->Component()->AddComponent<TextRenderer>();
	selectText->TextSetting("Aボタンを押して選択", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	select->SetDrawOrder(-350000);

	Object2D*  cursorObj = new Object2D();
	cursorObj->Init(VECTOR2F(780.0f, 400.0f),VECTOR2F(0, 0),VECTOR2F(1, 1),"Cursor");
	cursorText = cursorObj->Component()->AddComponent<TextRenderer>();
	cursorText->TextSetting("→","MonopinJRegular",".dft",WHITE,4, Font_ID::PAUSE_FONT);
	cursorObj->SetDrawOrder(-350000);
	
	pauseText->SetAlpha(0.0f);
	backText->SetAlpha(0.0f);
	titleText->SetAlpha(0.0f);
	selectText->SetAlpha(0.0f);
	cursorText->SetAlpha(0.0f);

	selectDelayTime = 0.0f;

	delayCountStart = false;
	baseSinRate = 0.0f;
}

PauseScreen::~PauseScreen()
{
	if (pauseText != nullptr) {
		pauseText = nullptr;
	}
	if (backText != nullptr) {
		backText = nullptr;
	}
	if (titleText != nullptr) {
		titleText = nullptr;
	}
	if (selectText != nullptr) {
		selectText = nullptr;
	}
	if (screen >= 0) {
		DeleteGraph(screen);
	}
}

void PauseScreen::Update()
{
	if (!pause) { //ポーズ中じゃないならリターン
		return;
	}
    if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.y >= 0.5f || InputManager::GetInstance()->KeyInputDown("PauseUp"))
    {
        pauseItem = Back;
		SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
    }

    if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.y <= -0.5f || InputManager::GetInstance()->KeyInputDown("PauseDown"))
    {
        pauseItem = Title;
		SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
    }

	if (InputManager::GetInstance()->KeyInputDown("PauseSelect"))
	{
		if (pauseItem == Title) {
			delayCountStart = true;
		}
		else {
			selectMenu = true;
		}
		SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
	}

	const float OFFSET_PLUS_RATE = 10.0f;

	const float ANIM_SPEED = 0.1f;

	if (delayCountStart) {
		animationTime += ANIM_SPEED;
		baseSinRate = sinf(animationTime);
	}
	else {
		animationTime += ANIM_SPEED;
		baseSinRate = sinf(animationTime);
	}
	

	float cursorOffset = baseSinRate * OFFSET_PLUS_RATE;
	if (pauseItem == Back)
	{
		cursorText->SetPos(VECTOR3(780.0f + cursorOffset,400.0f,0.0f));
	}
	else
	{
		cursorText->SetPos(VECTOR3(755.0f + cursorOffset,550.0f,0.0f));
	}

	if (delayCountStart) {
		selectDelayTime -= Time::DeltaTimeRate();
		if (selectDelayTime <= 0.0f) {
			selectMenu = true;
		}
		float alpha = 180.0f + sinf(animationTime * 5.0f) * 75.0f;
		selectText->SetAlpha(alpha);
	}
	
}

void PauseScreen::Draw()
{
	if (!pause) { //ポーズ中じゃないならリターン
		return;
	}

	//DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	SetDrawScreen(screen);
	DrawGraph(0,0,screen,FALSE);
	SetDrawScreen(DX_SCREEN_BACK);

	const int EDGE_NUM_X = 0;
	const int EDGE_NUM_Y = 0;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(EDGE_NUM_X, EDGE_NUM_Y, Screen::WIDTH, Screen::HEIGHT, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	/*DrawString(550, 250, "PAUSE", GetColor(255, 255, 255));*/

	if (pauseItem == PauseMenuItem::Back) {
		ActiveButtonState(backText, titleText);
	}
	else if (pauseItem == PauseMenuItem::Title) {
		ActiveButtonState(titleText, backText);
	}

	/*DrawString(560, 350, "Continue", resumeColor);
	DrawString(560, 420, "Title", titleColor);*/
}

void PauseScreen::PauseStart()
{
	pause = true;
	screen = MakeScreen(Screen::WIDTH, Screen::HEIGHT, TRUE);
	delayCountStart = false;

	SetDrawScreen(screen);
	GraphFilter(screen, DX_GRAPH_FILTER_GAUSS, 1024);
	DrawExtendGraph(0,0,Screen::WIDTH,Screen::HEIGHT,screen,FALSE);

	SetDrawScreen(DX_SCREEN_BACK);
	const float DELAY_TIME = 1.0f;
	selectDelayTime = DELAY_TIME;
	/*timeRate = Time::GetTimeRate();
	Time::ChangeDeltaRate(0.0f);*/
	//UiManager->SetUIDraw(false);
	selectMenu = false; 

	pauseText->SetAlpha((float)OPAQUE_COLOR);
	backText->SetAlpha((float)OPAQUE_COLOR);
	titleText->SetAlpha((float)OPAQUE_COLOR);
	selectText->SetAlpha((float)OPAQUE_COLOR);
	cursorText->SetAlpha((float)OPAQUE_COLOR);

	SoundManager::GetInstance()->ChangeVolumeSound(Sound_ID::PLAY_BGM,150);
}

bool PauseScreen::PauseFinish()
{
	pause = false;
	delayCountStart = false;
	if (screen >= 0) {
		DeleteGraph(screen);
		screen = -1;
	}
	/*Time::ChangeDeltaRate(timeRate);*/
	//UiManager->SetUIDraw(true);

	pauseText->SetAlpha(TRANSPARENT_COLOR_F);
	backText->SetAlpha(TRANSPARENT_COLOR_F);
	titleText->SetAlpha(TRANSPARENT_COLOR_F);
	selectText->SetAlpha(TRANSPARENT_COLOR_F);
	cursorText->SetAlpha(TRANSPARENT_COLOR_F);

	selectMenu = false;
	selectDelayTime = 0.0f;

	SoundManager::GetInstance()->ChangeVolumeSound(Sound_ID::PLAY_BGM, 250);

	if (pauseItem == Back) {
		return true;
	}
	else {
		return false;
	}

	
}

void PauseScreen::ActiveButtonState(TextRenderer* _activeButton, TextRenderer* _noActiveButton)
{
	const float BASE_SCALE = 1.35f;
	float scale = BASE_SCALE + baseSinRate * 0.08f;

	//アクティブ状態になるオブジェクトの修正
	_activeButton->SetColor(ORANGE);
	_activeButton->SetScale(VOne * scale);
	//非アクティブ状態になるオブジェクトの修正
	_noActiveButton->SetColor(WHITE);
	_noActiveButton->SetScale(VOne);
}
