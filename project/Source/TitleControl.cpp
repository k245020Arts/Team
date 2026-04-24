#include "TitleControl.h"
#include "TitlePlayer.h"

#include "../Source/Component/Object/Object3D.h"
#include "../Source/Common/Transitor/FadeTransitor.h"
#include "../Source/Common/Transitor/PushTransitor.h"
#include "../Source/Common/Transitor/WipeTransitor.h"
#include "../Source/Common/Transitor/StripTransitor.h"
#include "../Source/Common/Transitor/TitleTransitor.h"
#include "../Source/Common/Transitor/IrisTransitor.h"
#include "../Source/Common/Debug/Debug.h"
#include "../Source/Common/InputManager/inputManager.h"
#include "../Source/Common/ResourceLoader.h"
#include "../Source/Common/Sound/SoundManager.h"
#include "../Source/Common/Easing.h"
#include "State/StateManager.h"
#include "Screen.h"
#include "Stage/StageSelectData.h"

namespace {
	const float ALPHA_TIME = 1.0f;
}

TitleControl::TitleControl()
{

	hImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Title", ID::TITLE_BACK);
	titleImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "TitleImage", ID::TITLE);
	keyImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "TitlePush", ID::PUSH_BUTTON);
	font = LoadFontDataToHandle("data/font/MPlus2C.dft", 4);
	SoundManager::GetInstance()->AllDeleteSound();
	SoundManager::GetInstance()->TitleSceneLoad();
	SoundManager::GetInstance()->PlayBGM(Sound_ID::TITLE_BGM, true, true);
	firstCounter = 1.0f;
	pushCounter = 0.0f;
	exrate = 0.0f;
	moveButton = 0.0f;
	moveButton = 1.0f;
	progress = 0.0f;
	
	Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	player = obj->Component()->GetComponent<TitlePlayer>();

	SetDrawOrder(-100);
	stageID = 0;
	selectCounter = 0.0f;
	titleState = TITLE;
	alpha = 255;
}

TitleControl::~TitleControl()
{
	Debug::ClearLogger();
	DeleteFontToHandle(font);
}

void TitleControl::Update()
{
	//プレイヤーの回避アニメーションが終わったら、シーンを遷移
	if (progress >= 0.99f)
	{
		Time::ChangeDeltaRate(1);
		FindGameObject<FadeTransitor>()->StartTransitor("PLAY", 1.0f);
	}

	switch (titleState)
	{
	case TITLE:
		TitleUpdate();
		break;
	case STAGE_SELECT:
		StageUpdate();
		break;
	default:
		break;
	}

	if (alphaTime > 0.0f) {
		alphaTime -= Time::DeltaTimeRate();
		if (alphaTime <= 0.0f) {
			alphaTime = 0.0f;
		}
		float rate = alphaTime / ALPHA_TIME;
		switch (titleState)
		{
		case TITLE:
			alpha = Easing::Lerp(255, 0, rate);
			break;
		case STAGE_SELECT:
			alpha = Easing::Lerp(0, 255, rate);
			break;
		default:
			break;
		}
	}
}

float TitleControl::GetNowProgress()
{
	return progress;
}

void TitleControl::SetNowProgress(float nowProgress)
{
	progress = nowProgress;
}

void TitleControl::StageSelect()
{
	if (selectCounter > 0.0f) {
		return;
	}
	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.x >= 0.5f || InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_RIGHT)) {
		stageID++;
		int stageMax = StageSelectData::GetInstance()->GetStageMax() - 1;
		if (stageID >= stageMax) {
			stageID = stageMax;
		}
		StageSelectData::GetInstance()->SetStageID(stageID);
		selectCounter = 0.5f;
	}

	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.x <= -0.5f || InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_LEFT)) {
		stageID--;
		if (stageID <= 0) {
			stageID = 0;
		}
		StageSelectData::GetInstance()->SetStageID(stageID);
		selectCounter = 0.5f;
	}
}

#define BLACK_TEXTURE SetDrawBright(0, 0, 0)
#define DEFAULT_TEXTURE SetDrawBright(255, 255, 255)

void TitleControl::Draw()
{
	/*DrawGraph(0, 0, hImage, true);*/
	DrawGraph(750, 100, titleImage, true);
	if (progress > 0)
		return;

	StageData stageData = StageSelectData::GetInstance()->GetNowStageData();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(Screen::WIDTH / 2, 850, (double)exrate * 2, 0.0, keyImage, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	DrawExtendFormatStringToHandle(Screen::WIDTH / 2 + 100, 850, 1.0, 1.0, 0xffffff, font, stageData.name.c_str());
	int width = GetDrawExtendFormatStringWidthToHandle(1.0f, font, stageData.name.c_str());

	int stageMax = StageSelectData::GetInstance()->GetStageMax() - 1;
	if (stageID == stageMax) {
		BLACK_TEXTURE;
	}
	
	DrawExtendFormatStringToHandle(Screen::WIDTH / 2 + width + 150, 850, 1.0, 1.0, 0xffffff, font,"→");
	DEFAULT_TEXTURE;

	if (stageID == 0) {
		BLACK_TEXTURE;
	}
	DrawExtendFormatStringToHandle(Screen::WIDTH / 2 - 50, 850, 1.0, 1.0, 0xffffff, font,"←");
	DEFAULT_TEXTURE;

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleControl::TitleUpdate()
{
	if (InputManager::GetInstance()->KeyInputDown("SceneChange") && progress == 0) // 押したら
	{
		SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
		titleState = STAGE_SELECT;
		alphaTime = ALPHA_TIME;
	}
	if (firstCounter > 0.0f)
	{
		firstCounter -= Time::DeltaTimeRate();
		if (firstCounter <= 0.0f)
		{
			firstCounter = 0.0f;
		}
		float rate = firstCounter / 1.0f;
		exrate = Easing::EaseIn(0.0f, 0.8f, 0.8f - rate);
	}
	else
	{
		if (pushCounter > 0.0f) // 押したあと
		{
			float rate = pushCounter / 0.5f;
			exrate = Easing::Sin90Cube(0.5f, 0.8f, 0.8f - rate);
		}
		else
		{
			float rate = 0.0f;
			if (moveButton > 0.0f)
			{
				moveButton -= Time::DeltaTimeRate();
				rate = moveButton / 1.0f;
				if (moveButton <= 0.0f)
					moveButton = -1.0f;

			}
			else if (moveButton < 0.0f)
			{
				moveButton += Time::DeltaTimeRate();
				rate = moveButton / -1.0f;
				rate = 1 - rate;
				if (moveButton >= 0.0f)
					moveButton = 1.0f;
			}
			exrate = Easing::EaseInOut(0.4f, 0.5f, rate);
		}
	}
}

void TitleControl::StageUpdate()
{
	if (InputManager::GetInstance()->KeyInputDown("SceneChange") && progress == 0) // 押したら
	{
		SoundManager::GetInstance()->AllDeleteSound();
		SoundManager::GetInstance()->TitleSceneLoad();

		SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
		SoundManager::GetInstance()->PlaySe(Sound_ID::JUST_AVOID_SOUND);
		//sound->BaseVolumeChange(Sound_ID::JUST_AVOID_SUCCESS);
		SoundManager::GetInstance()->PlaySe(Sound_ID::JUST_AVOID_SUCCESS);
		SoundManager::GetInstance()->PlaySe(Sound_ID::V_P_JUST_AVOID);

		player->playerCom.stateManager->ChangeState(StateID::PLAYER_AVOID_S);
		StageSelectData::GetInstance()->SetStageID(stageID);
	}

	StageSelect();

	if (selectCounter >= 0.0f) {
		selectCounter -= Time::DeltaTimeRate();
	}
}

