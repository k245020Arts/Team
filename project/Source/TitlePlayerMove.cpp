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


TitlePlayerMove::TitlePlayerMove()
{
	animId = ID::P_ANIM_AVOID;
	string = Function::GetClassNameC<TitlePlayerMove>();;
	blurScreen = FindGameObject<BlurScreen>();
}

TitlePlayerMove::~TitlePlayerMove()
{
}

void TitlePlayerMove::Update()
{
	TitlePlayer* player = GetBase<TitlePlayer>();
	if (startTimer >= 0.0f)
	{
		startTimer -= Time::DeltaTime();
		return;
	}
	Time::ChangeDeltaRate(0.4f);
	easingCount += Time::DeltaTime() * 0.8f;
	if (easingCount >= 1.0f)
	{
		easingCount = 1.0f;

	}
	//JustAvoidRotation();
	float rate = Easing::SinCube(1.7f, 0.7f, easingCount);
	float count = easingCount * 2.0f;
	if (count > 1.0f)
	{
		count = 1.0f;
	}
	//回避の速度の調整
	float speed = Easing::EaseInOut(9000.0f, 2000.0f, count);
	player->playerCom.anim->SetPlaySpeed(rate);

	//player->playerCom.player->Avoid(speed, 6000.0f, cameraAngle, 0.0f);
	//カメラの追従速度を早くする
	float leap = Easing::EaseIn(cameraLeap, 0.2f, easingCount / 2.0f);
	//player->playerCom.camera->CameraLeapSet(leap);

	if (player->playerCom.anim->IsFinish())
	{
		//player->playerCom.player->AvoidFinishState();
		player->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
	}
	//再生速度を遅くしている
	if (player->playerCom.anim->GetCurrentFrame() >= 14.0f && player->playerCom.anim->GetCurrentFrame() <= 17.0f)
	{
		Time::ChangeDeltaRate(0.1f);
		player->playerCom.anim->SetPlaySpeed(2.0f);
		//player->noDamage = false;
		//Time::ChangeDeltaRate(0.1f);
	}
	//そこから早くする
	else if (player->playerCom.anim->GetCurrentFrame() >= 17.0f)
	{
		player->playerCom.anim->SetPlaySpeed(2.0f);

	}
	else
	{

	}
	//ジャスト回避攻撃を遷移させる
	if (player->playerCom.InputManager->KeyInput("attack"))
	{
		attack = true;
	}
	if (easingCount >= 0.5f)
	{
		//if (attack)
		//{
		//	if (true /*player->largeJustAvoid*/)
		//	{
		//		player->playerCom.stateManager->ChangeState(StateID::PLAYER_JUST_AVOID_ATTACK2_S);
		//	}
		//	else
		//	{
		//		player->playerCom.stateManager->ChangeState(StateID::PLAYER_ATTACK1_S);
		//	}

		//}
	}
	//残像を付ける
	if (player->playerCom.anim->AnimEventCan())
	{

		if (num % 30 == 0)
		{
			player->playerCom.blur->MosionStart(0.3f, 0.04f, animId, 1);

			volume -= 40.0f;
			if (volume <= 120.0f)
			{
				player->playerCom.sound->PlaySe(Sound_ID::JUST_AVOID_SUCCESS);
				player->playerCom.sound->ChangeSound(Sound_ID::JUST_AVOID_SUCCESS, volume);
			}
		}
		num++;
	}

}

void TitlePlayerMove::Draw()
{
}

void TitlePlayerMove::Start()
{
	TitlePlayer* player = GetBase<TitlePlayer>();
	player->playerCom.anim->Play(animId, blendSpeed);
	noStateChange = false;

	//

	//PlayerStateBase::Start();
	//player->largeJustAvoid = true;
	easingCount = 0.0f;

	player->playerCom.anim->SetPlaySpeed(1.0f);
	player->playerCom.sound->FeedInOut(Sound_ID::TITLE_BGM, 0.2f);

	//JustAvoidShadow();
	//cameraAngle = player->playerCom.camera->GetCameraTransform()->rotation.y;

	//スティックをみる
	//player->playerCom.player->PlayerStickInput();
	//cameraRotation = player->playerCom.camera->GetCameraTransform()->rotation;

	//音を鳴らす
	player->playerCom.sound->PlaySe(Sound_ID::JUST_AVOID_SOUND);
	volume = 200;
	player->playerCom.sound->BaseVolumeChange(Sound_ID::JUST_AVOID_SUCCESS);
	player->playerCom.sound->PlaySe(Sound_ID::JUST_AVOID_SUCCESS);

	//エフェクトの再生
	player->playerCom.effect->CreateEffekseer(Transform(VECTOR3(Screen::WIDTH / 2.0f, Screen::HEIGHT / 2.0f, 0), VZero, VOne * 3.0f), nullptr, Effect_ID::JUST_AVOID_EFFECT, 1.0f, false);
	player->playerCom.effect->CreateEffekseer(Transform(MV1GetFramePosition(Load::GetHandle(ID::IDType::P_MODEL), 12), VZero, VOne * 1.0f), nullptr, Effect_ID::PLAYER_FLASH, 1.0f);
	//p->playerCom.effect->CreateEffekseer(Transform(VECTOR3(0, 100, 0), VZero, VOne), p->playerCom.player->GetPlayerObj(), ID::PLAYER_AURA, 10.0f);

	//プレイヤーを青くする
	player->playerCom.color->setRGB(Color::Rgb(0, 0, 255, 255));
	player->playerCom.controller->ControlVibrationStartFrame(50, 10);

	//カメラの追従速度を遅くする

	//敵の攻撃を遅くする
	Time::ChangeDeltaRate(0.6f);//世界を遅くする
	cameraLeap = 0.02f;


	//player->playerCom.camera->CameraLeapSet(cameraLeap);
	//player->playerCom.camera->TargetSet(player->playerCom.hitObj);
	player->playerCom.sound->PlaySe(Sound_ID::V_P_JUST_AVOID);


	//カメラの状態遷移
	//player->playerCom.camera->ChangeStateCamera(StateID::JUST_AVOID_CAMERA_S);
	attack = false;


	//Time::ChangeDeltaRate(0.01f);
	player->playerCom.anim->SetFrame(12.0f);
	num = 0;

	//ブラースクリーンを再生する
	//blurScreen->Play(0.25f, 0.1f);
	startTimer = 0.0f;

}

void TitlePlayerMove::Finish()
{
}
