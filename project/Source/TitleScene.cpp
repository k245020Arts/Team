#include "TitleScene.h"
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

TitleScene::TitleScene()
{
	//hImage = LoadGraph("data/image/SlashLocus.png");
	input = FindGameObject<InputManager>();

	hImage = Load::LoadImageGraph(Load::IMAGE_PATH + "Title", ID::TITLE_BACK);
	titleImage = Load::LoadImageGraph(Load::IMAGE_PATH + "TitleImage", ID::TITLE);
	keyImage = Load::LoadImageGraph(Load::IMAGE_PATH + "TitlePush", ID::PUSH_BUTTON);
	sound = FindGameObject<SoundManager>();
	sound->TitleSceneLoad();
	sound->PlayBGM(Sound_ID::TITLE_BGM, true, true);
	firstCounter = 1.0f;
	pushCounter = 0.0f;
	exrate = 0.0f;
	moveButton = 0.0f;
	moveButton = 1.0f;
}

TitleScene::~TitleScene()
{
	//DeleteGraph(hImage);
	Load::AllDelete();
	//FindGameObject<SoundManager>()->AllDeleteSound();
}

void TitleScene::Update()
{
	if (firstCounter > 0.0f) {
		firstCounter -= Time::DeltaTimeRate();
		if (firstCounter <= 0.0f) {
			firstCounter = 0.0f;
		}
		float rate = firstCounter / 1.0f;
		exrate = Easing::EaseInExpo(0.0f, 1.0f, 1.0f - rate);
	}
	else {
		if (input->KeyInputDown("SceneChange")) {
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
			exrate = Easing::Sin90Cube(0.5f, 1.0f, 1.0f - rate);
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
			
			exrate = Easing::EaseIn(0.6f, 1.0f, rate);
		}
		
	}
	
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	DrawGraph(0, 0, hImage, true);
	DrawGraph(750, 100, titleImage, true);
	DrawRotaGraph(1000, 800, (double)exrate,0.0,keyImage, true);

	/*DrawBox(0, 0, 1920, 1080, 0x111111, true);
	extern const char* Version();*/
	/*DrawString(0, 20, Version(), GetColor(255,255,255));
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	DrawFormatString(100, 100, GetColor(255,255,255), "%4.1f", 1.0f / Time::DeltaTime());
	DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));*/
	//DrawGraph(100, 100, hImage, true);
}
