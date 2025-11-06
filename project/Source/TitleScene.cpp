#include "TitleScene.h"
#include "FadeTransitor.h"
#include "PushTransitor.h"
#include "WipeTransitor.h"
#include "StripTransitor.h"
#include "TitleTransitor.h"
#include "IrisTransitor.h"
#include "Debug.h"
#include "inputManager.h"
#include "LoadManager.h"
#include "SoundManager.h"

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
}

TitleScene::~TitleScene()
{
	//DeleteGraph(hImage);
	Load::AllDelete();
	FindGameObject<SoundManager>()->AllDeleteSound();
}

void TitleScene::Update()
{
	if (input->KeyInputDown("SceneChange")) {
		sound->PlaySe(Sound_ID::PUSH);
		FindGameObject<FadeTransitor>()->StartTransitor("PLAY", 1.0f);
		//Debug::DebugLog(std::to_string(GetMemory()));
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	DrawGraph(0, 0, hImage, true);
	DrawGraph(750, 100, titleImage, true);
	DrawGraph(600, 800, keyImage, true);

	/*DrawBox(0, 0, 1920, 1080, 0x111111, true);
	extern const char* Version();*/
	/*DrawString(0, 20, Version(), GetColor(255,255,255));
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	DrawFormatString(100, 100, GetColor(255,255,255), "%4.1f", 1.0f / Time::DeltaTime());
	DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));*/
	//DrawGraph(100, 100, hImage, true);
}
