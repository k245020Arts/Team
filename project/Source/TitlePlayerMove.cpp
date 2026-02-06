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
}

TitlePlayerMove::~TitlePlayerMove()
{
}

void TitlePlayerMove::Start()
{
	TitlePlayer* player = GetBase<TitlePlayer>();
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

	//敵の攻撃を遅くする
	//Time::ChangeDeltaRate(0.6f);//世界を遅くする
	cameraLeap = 0.02f;

	player->playerCom.sound->PlaySe(Sound_ID::V_P_JUST_AVOID);


	player->playerCom.anim->SetFrame(12.0f);
	num = 0;

	//ブラースクリーンを再生する
	blurScreen->Play(1.0f, 0.1); //0.25 , 0.1f
	startTimer = 0.0f;

}


void TitlePlayerMove::Update()
{
	TitlePlayer* player = GetBase<TitlePlayer>();
	if (startTimer >= 0.0f)
	{
		startTimer -= Time::DeltaTime();
		return;
	}
	Time::ChangeDeltaRate(Easing::Lerp(0.001f, 0.5f, easingCount / 1.1f)); // 0.4
	easingCount += Time::DeltaTime() * 0.8f;
	if (easingCount >= 1.0f)
	{
		easingCount = 1.0f;

	}

	float rate = Easing::SinCube(1.7f, 0.7f, easingCount);
	float count = easingCount * 2.0f;
	if (count > 1.0f)
	{
		count = 1.0f;
	}
	//回避の速度の調整
	float speed = Easing::EaseInOut(9000.0f, 2000.0f, count);
	player->playerCom.anim->SetPlaySpeed(rate);

	Avoid(speed, 6000.0f, cameraAngle);

	if (player->playerCom.anim->IsFinish())
	{
		player->playerCom.physics->SetVelocity(VZero);
		player->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
		player->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}
}

void TitlePlayerMove::Draw()
{
}


void TitlePlayerMove::Avoid(float _speed, float _speedMax, float cameraAngle)
{
	TitlePlayer* player = GetBase<TitlePlayer>();

	//回避の共通処理
	VECTOR dir = VZero;
	dir = VECTOR3(0.0f, 0.0f, _speed) * MGetRotY(player->playerTransform->rotation.y);
	dir.y = 0.0f;
	player->playerCom.physics->SetVelocity(dir);
}


void TitlePlayerMove::Finish()
{
}
