#include "TitleScene.h"
#include "FadeTransitor.h"
#include "PushTransitor.h"
#include "WipeTransitor.h"
#include "StripTransitor.h"
#include "TitleTransitor.h"
#include "IrisTransitor.h"
#include "Debug.h"

TitleScene::TitleScene()
{
	//hImage = LoadGraph("data/image/SlashLocus.png");
}

TitleScene::~TitleScene()
{
	//DeleteGraph(hImage);
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_P)) {
		FindGameObject<FadeTransitor>()->StartTransitor("PLAY", 0.05f);
		//Debug::DebugLog(std::to_string(GetMemory()));
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	DrawBox(0, 0, 1920, 1080, 0x111111, true);
	extern const char* Version();
	DrawString(0, 20, Version(), GetColor(255,255,255));
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	DrawFormatString(100, 100, GetColor(255,255,255), "%4.1f", 1.0f / Time::DeltaTime());
	DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));
	//DrawGraph(100, 100, hImage, true);
}
