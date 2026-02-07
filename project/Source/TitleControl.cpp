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
#include "../Source/Common/LoadManager.h"
#include "../Source/Common/Sound/SoundManager.h"
#include "../Source/Common/Easing.h"
#include "State/StateManager.h"
#include "Screen.h"

TitleControl::TitleControl()
{
	input = FindGameObject<InputManager>();

	hImage = Load::LoadImageGraph(Load::IMAGE_PATH + "Title", ID::TITLE_BACK);
	titleImage = Load::LoadImageGraph(Load::IMAGE_PATH + "TitleImage", ID::TITLE);
	keyImage = Load::LoadImageGraph(Load::IMAGE_PATH + "TitlePush", ID::PUSH_BUTTON);
	sound = FindGameObject<SoundManager>();
	sound->AllDeleteSound();
	sound->TitleSceneLoad();
	sound->PlayBGM(Sound_ID::TITLE_BGM, true, true);
	firstCounter = 1.0f;
	pushCounter = 0.0f;
	exrate = 0.0f;
	moveButton = 0.0f;
	moveButton = 1.0f;
	progress = 0.0f;
	
	Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	player = obj->Component()->GetComponent<TitlePlayer>();

	SetDrawOrder(-100);
}

TitleControl::~TitleControl()
{
	Debug::ClearLogger();
}

void TitleControl::Update()
{
	//プレイヤーの回避アニメーションが終わったら、シーンを遷移
	if (progress >= 0.99f)
	{
		Time::ChangeDeltaRate(1);
		FindGameObject<FadeTransitor>()->StartTransitor("PLAY", 1.0f);
	}

	if (input->KeyInputDown("SceneChange") && progress == 0) // 押したら
	{
		sound->TitleSceneLoad();

		sound->PlaySe(Sound_ID::PUSH);
		sound->PlaySe(Sound_ID::JUST_AVOID_SOUND);
		//sound->BaseVolumeChange(Sound_ID::JUST_AVOID_SUCCESS);
		sound->PlaySe(Sound_ID::JUST_AVOID_SUCCESS);
		sound->PlaySe(Sound_ID::V_P_JUST_AVOID);

		player->playerCom.stateManager->ChangeState(StateID::PLAYER_AVOID_S);
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

float TitleControl::GetNowProgress()
{
	return progress;
}

void TitleControl::SetNowProgress(float nowProgress)
{
	progress = nowProgress;
}

void TitleControl::Draw()
{
	/*DrawGraph(0, 0, hImage, true);*/
	DrawGraph(750, 100, titleImage, true);
	if (progress > 0)
		return;
	DrawRotaGraph(Screen::WIDTH / 2, 850, (double)exrate * 2,0.0,keyImage, true);
}

