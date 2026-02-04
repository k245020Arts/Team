#include "TitleControl.h"
#include "TitlePlayer.h"
#include "TitlePlayerMove.h"
#include "Component/Animator/Animator.h"
#include "Common/Easing.h"
#include "Player/Player.h"

#include "Common/InputManager/ControllerInputManager.h"
#include "Component/Physics/Physics.h"
#include "Camera/Camera.h"
#include "Common/Effect/EffectManager.h"
#include "Screen.h"
#include "Common/LoadManager.h"
#include "Common/Sound/SoundManager.h"
#include "Component/Color/color.h"
#include "Component/MotionBlur/MotionBlur.h"
#include "Common/BlurScreen.h"
#include "Common/InputManager/InputManager.h"
#include "State/StateManager.h"


TitlePlayerMove::TitlePlayerMove()
{
	animId = ID::P_ANIM_AVOID;
	string = Function::GetClassNameC<TitlePlayerMove>();;
	blurScreen = FindGameObject<BlurScreen>();
	titleCtrl = FindGameObject<TitleControl>();
}

TitlePlayerMove::~TitlePlayerMove()
{
}

void TitlePlayerMove::Start()
{
	player = GetBase<TitlePlayer>();
	player->playerCom.anim->Play(animId, blendSpeed);
	noStateChange = false;

	easingCount = 0.0f;

	player->playerCom.anim->SetPlaySpeed(1.0f);
	//player->playerCom.sound->FeedInOut(Sound_ID::TITLE_BGM, 0.2f);

	//音を鳴らす
	volume = 200;

	player->playerCom.sound->PlaySe(Sound_ID::JUST_AVOID_SOUND);
	player->playerCom.sound->BaseVolumeChange(Sound_ID::JUST_AVOID_SUCCESS);
	player->playerCom.sound->PlaySe(Sound_ID::JUST_AVOID_SUCCESS);

	//エフェクトの再生
	player->playerCom.effect->CreateEffekseer(Transform(VECTOR3(Screen::WIDTH / 2.0f, Screen::HEIGHT / 2.0f, 0), VZero, VOne * 3.0f), nullptr, Effect_ID::JUST_AVOID_EFFECT, 10.0f, false);
	player->playerCom.effect->CreateEffekseer(Transform(VECTOR3(0, 150, 0), VZero, VOne * 1.0f), obj, Effect_ID::PLAYER_FLASH, 10.0f);
	player->playerCom.effect->CreateEffekseer(Transform(VECTOR3(0, 100, 0), VZero, VOne), obj, Effect_ID::PLAYER_AURA, 10.0f);

	//プレイヤーを青くする
	player->playerCom.color->setRGB(Color::Rgb(0, 0, 255, 255));
	player->playerCom.controller->ControlVibrationStartFrame(50, 10);

	cameraLeap = 0.02f;

	player->playerCom.sound->PlaySe(Sound_ID::V_P_JUST_AVOID);

	player->playerCom.anim->SetFrame(12.0f);
	num = 0;
}


void TitlePlayerMove::Update()
{
	//回避の移動
	const float MOVE_DISTANCE = 2000;
	const float COEFFICIENT_AVOIDSPEED = 1.45f;
	Avoid(COEFFICIENT_AVOIDSPEED * MOVE_DISTANCE);

	//現在の進捗を更新
	progress =  player->obj->Component()->GetComponent<Transform>()->position.z / MOVE_DISTANCE;
	titleCtrl = FindGameObject<TitleControl>();
	titleCtrl->SetNowProgress(progress);

	//タイムスケールの変更
	const float TIMESCALE_MIN = 0.1f;
	const float TIMESCALE_MAX = 1.0f;
	float nowTimescalse = Easing::EaseIn(TIMESCALE_MIN, TIMESCALE_MAX, progress);
	Time::ChangeDeltaRate(nowTimescalse);

	//進捗が一定以上なら、ブラースクリーンを表示させる
	if (progress >= 0.75f)
		blurScreen->Play(1.0f, 0.1f);

	//アニメーションが終わったら、進捗を強制的に1に変更
	if (player->playerCom.anim->IsFinish())
		titleCtrl->SetNowProgress(1);
}

void TitlePlayerMove::Avoid(float speed)
{
	player->playerCom.physics->SetVelocity(VECTOR3(0, 0, speed));
}

void TitlePlayerMove::Draw()
{
}

void TitlePlayerMove::Finish()
{
}
