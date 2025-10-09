#include "playerjustAvoid.h"
#include "controllerInputManager.h"
#include "playerStateManager.h"
#include "Animator.h"
#include "Physics.h"
#include "Easing.h"
#include "player.h"
#include "camera.h"
#include "EffectManager.h"
#include "Screen.h"
#include "LoadManager.h"
#include "SoundManager.h"
#include "color.h"
#include "MotionBlur.h"
#include "BlurScreen.h"
#include "inputManager.h"

PlayerJustAvoid::PlayerJustAvoid()
{

	string = string = Function::GetClassNameC<PlayerJustAvoid>();;
	animId = ID::P_ANIM_JUST_AVOID;
	id = ID::P_ANIM_JUST_AVOID;
	attack = false;
	blurScreen = FindGameObject<BlurScreen>();
}

PlayerJustAvoid::~PlayerJustAvoid()
{
}

void PlayerJustAvoid::Update()
{
	Player* p = GetBase<Player>();
	if (startTimer >= 0.0f) {
		startTimer -= Time::DeltaTime();
		return;
	}
	Time::ChangeDeltaRate(0.4f);
	p->playerCom.enemyObj->SetObjectTimeRate(0.6f);
	easingCount += Time::DeltaTime() * 0.8f;
	if (easingCount >= 1.0f) {
		easingCount = 1.0f;
	}
	JustAvoidRotation();
	float rate = Easing::SinCube(1.7f, 0.7f, easingCount);
	float count = easingCount * 2.0f;
	if (count > 1.0f) {
		count = 1.0f;
	}
	//回避の速度の調整
	float speed = Easing::EaseInOut(9000.0f, 2000.0f, count);
	p->playerCom.anim->SetPlaySpeed(rate);
	p->playerCom.player->Avoid(speed, 6000.0f, cameraAngle,0.0f);
	//カメラの追従速度を早くする
	float leap = Easing::EaseIn(cameraLeap, 0.2f, easingCount / 2.0f);
	p->playerCom.camera->CameraLeapSet(leap);
	if (p->playerCom.anim->IsFinish())
	{
		p->playerCom.player->AvoidFinishState();
		p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
		p->playerCom.enemyObj->SetObjectTimeRate(1.0f);
	}
	//再生速度を遅くしている
	if (p->playerCom.anim->GetCurrentFrame() >= 14.0f && p->playerCom.anim->GetCurrentFrame() <= 17.0f){
		//p->playerCom.player->GetPlayerObj()->SetObjectTimeRate(0.5f);
		Time::ChangeDeltaRate(0.1f);
	}
	//そこから早くする
	else if (p->playerCom.anim->GetCurrentFrame() >= 17.0f) {
		p->playerCom.anim->SetPlaySpeed(2.0f);
	}
	else {
		p->playerCom.player->GetPlayerObj()->SetObjectTimeRate(1.0f);
	}
	//ジャスト回避攻撃を遷移させる
	if (p->playerCom.InputManager->KeyInput("attack")) {
		attack = true;
	}
	if (easingCount >= 0.5f) {
		if (attack) {
			p->playerCom.stateManager->ChangeState(ID::P_ANIM_JUST_AVOID_ATTACK3);
		}
	}
	//残像を付ける
	if (p->playerCom.anim->AnimEventCan()) {
		
		if (num % 30 == 0) {
			p->playerCom.blur->MosionStart(0.3f, 0.04f, animId, 1);
		}
		num++;
	}
	
}

void PlayerJustAvoid::Draw()
{
	/*if (avoidShadowStart) {
		shadowAddTime += p->playerCom.player->GetPlayerObj()->GetObjectTimeRate();
		if (shadowNum < SHADOW_NUM_MAX && shadowAddTime >= 0.015f) {
			shadowTransform[shadowNum] = *p->playerCom.player->GetPlayerTransform();
			shadowNum++;
			shadowAddTime = 0.0f;
		}
		for (int i = 0; i < SHADOW_NUM_MAX; i++) {
			if (i >= shadowNum) {
				anim[i]->Update();
			}
			else {
				MV1SetMatrix(hModel[i], shadowTransform[i].GetMatrix());
				MV1SetDifColorScale(hModel[i], GetColorF(1.0f, 1.0f, 0.0f, 0.2f));
				MV1DrawModel(hModel[i]);
			}
		}
	}*/
}

void PlayerJustAvoid::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	easingCount = 0.0f;
	Time::ChangeDeltaRate(0.4f);//世界を遅くする
	p->playerCom.anim->SetPlaySpeed(1.0f);
	//JustAvoidShadow();
	cameraAngle = p->playerCom.camera->GetCameraTransform()->rotation.y;
	//スティックをみる
	p->playerCom.player->PlayerStickInput();
	cameraRotation = p->playerCom.camera->GetCameraTransform()->rotation;
	//音を鳴らす
	p->playerCom.sound->PlaySe(Sound_ID::JUST_AVOID_SOUND);
	p->playerCom.sound->PlaySe(Sound_ID::JUST_AVOID_SUCCESS);
	//エフェクトの再生
	p->playerCom.effect->CreateEffekseer(Transform(VECTOR3(Screen::WIDTH / 2.0f , Screen::HEIGHT / 2.0f, 0), VZero, VOne * 3.0f), nullptr, Effect_ID::JUST_AVOID_EFFECT, 1.0f, false);
	p->playerCom.effect->CreateEffekseer(Transform(MV1GetFramePosition(Load::GetHandle(ID::IDType::P_MODEL), 12), VZero, VOne * 1.0f), nullptr, Effect_ID::PLAYER_FLASH, 1.0f);
	//p->playerCom.effect->CreateEffekseer(Transform(VECTOR3(0, 100, 0), VZero, VOne), p->playerCom.player->GetPlayerObj(), ID::PLAYER_AURA, 10.0f);
	//プレイヤーを青くする
	p->playerCom.color->setRGB(Color::Rgb(0,0,255,255));
	p->playerCom.controller->ControlVibrationStartFrame(50, 10);
	//カメラの追従速度を遅くする
	cameraLeap = 0.02f;
	p->playerCom.camera->CameraLeapSet(cameraLeap);
	p->playerCom.sound->PlaySe(Sound_ID::V_P_JUST_AVOID);
	//カメラの状態遷移
	p->playerCom.camera->ChangeStateCamera(ID::P_ANIM_JUST_AVOID);
	attack = false;
	//敵の攻撃を遅くする
	p->playerCom.enemyObj->SetObjectTimeRate(0.6f);
	p->playerCom.enemyObj->SetObjectTimeRate(0.0f);
	Time::ChangeDeltaRate(0.0f);
	p->playerCom.anim->SetFrame(12.0f);
	num = 0;
	//ブラースクリーンを再生する
	blurScreen->Play(0.25f, 0.25f);
	startTimer = 0.0f;
}

void PlayerJustAvoid::Finish()
{
	Player* p = GetBase<Player>();
	Time::ChangeDeltaRate(1.0f);
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->playerCom.player->PlayerStickInput();
	p->playerCom.camera->CameraLeapSet(0.2f);
	p->playerCom.enemyObj->SetObjectTimeRate(1.0f);
}

void PlayerJustAvoid::JustAvoidShadow()
{
	/*avoidShadowStart = true;
	shadowNum = 0;
	shadowAddTime = 0.0f;
	for (int i = 0; i < SHADOW_NUM_MAX; i++) {
		anim[i]->Play(ID::P_ANIM_AVOID);
		anim[i]->Init(p->playerCom.player->GetPlayerObj());
	}*/
}

void PlayerJustAvoid::JustAvoidRotation()
{
	Player* p = GetBase<Player>();
	//プレイヤーの角度の回転処理
	Transform* pl = p->playerCom.player->GetPlayerTransform();
	VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(pl->rotation.y);
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(pl->rotation.y);
	VECTOR3 target = p->playerCom.player->GetWalkAngle() * MGetRotY(cameraRotation.y);
	float dot = VDot(target.Normalize(), forward.Normalize());
	if (dot >= cosf(5.0f * DegToRad)) {
		float inRot = atan2f(target.x, target.z);
		pl->rotation.y = inRot;
	}
	else if (VDot(right, target) > 0) {
		pl->rotation.y += 5.0f * DegToRad;
		
	}
	else {
		pl->rotation.y -= 5.0f * DegToRad;
	}
}
