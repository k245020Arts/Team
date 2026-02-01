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

TitleControl::TitleControl()
{
	input = FindGameObject<InputManager>();

	hImage = Load::LoadImageGraph(Load::IMAGE_PATH + "Title", ID::TITLE_BACK);
	titleImage = Load::LoadImageGraph(Load::IMAGE_PATH + "TitleImage", ID::TITLE);
	keyImage = Load::LoadImageGraph(Load::IMAGE_PATH + "TitlePush", ID::PUSH_BUTTON);
	sound = FindGameObject<SoundManager>();
	sound->TitleSceneLoad();
	sound->PlaySceneLoad(); // 
	sound->PlayBGM(Sound_ID::TITLE_BGM, true, true);
	firstCounter = 1.0f;
	pushCounter = 0.0f;
	exrate = 0.0f;
	moveButton = 0.0f;
	moveButton = 1.0f;

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
		if (input->KeyInputDown("SceneChange")) 
		{
			sound->PlaySe(Sound_ID::PUSH);

			pushCounter = 0.5f;
		}
		if (pushCounter > 0.0f) {
			pushCounter -= Time::DeltaTimeRate();
			if (pushCounter <= 0.0f) {
				pushCounter = 0.0f;
				FindGameObject<FadeTransitor>()->StartTransitor("PLAY", 1.0f);
			}
			float rate = pushCounter / 0.5f;
			exrate = Easing::Sin90Cube(0.5f, 0.8f, 0.8f - rate);
		}
		else {
			float rate = 0.0f;
			if (moveButton > 0.0f) {
				moveButton -= Time::DeltaTimeRate();
				rate = moveButton / 1.0f;
				if (moveButton <= 0.0f) {
					moveButton = -1.0f;
				}

			}
			else if (moveButton < 0.0f) {
				moveButton += Time::DeltaTimeRate();
				rate = moveButton / -1.0f;
				rate = 1 - rate;
				if (moveButton >= 0.0f) {
					moveButton = 1.0f;
				}

			}

			exrate = Easing::EaseIn(0.4f, 0.5f, rate);
		}

	}
}

void TitleControl::Draw()
{
	/*DrawGraph(0, 0, hImage, true);*/
	DrawGraph(750, 100, titleImage, true);
	DrawRotaGraph(1000, 900, (double)exrate,0.0,keyImage, true);
}
