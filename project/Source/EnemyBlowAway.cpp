#include "EnemyBlowAway.h"
#include "Physics.h"
#include "Animator.h"
#include "enemy.h"
#include "Shaker.h"
#include "EnemyStateManager.h"
#include "transform.h"
#include "camera.h"
#include "controllerInputManager.h"
#include "color.h"
#include "SoundManager.h"

EnemyBlowAway::EnemyBlowAway()
{
	string = Function::GetClassNameC<EnemyBlowAway>();
	animId = ID::E_DAMAGE; 
	id = ID::E_BLOWAWAY;
	GravityAdd = 0.0f;
	
}

EnemyBlowAway::~EnemyBlowAway()
{
}

void EnemyBlowAway::Update()
{
	Enemy* e = GetBase<Enemy>();
	//敵が落ちていて落下が終わったらカメラを振動
	if (e->eCom.anim->GetCurrentID() == ID::GetID(ID::E_FALL)) {
		if (e->eCom.enemy->GetEnemyTransform()->position.y <= 0.0f) {
			e->eCom.anim->Play(ID::E_GETUP);
			e->eCom.shaker->ShakeFinish();
			e->eCom.camera->CameraShake(VECTOR3(100, 150, 100), Shaker::HEIGHT_SHAKE, false , 0.5f);
			e->eCom.control->ControlVibrationStartFrame(200, 30);
			e->eCom.sound->PlaySe(Sound_ID::ENEMY_FALL);
			e->eCom.sound->PlaySe(Sound_ID::V_E_DAMAGE4);
		}
	}
	//ダメージの赤点滅
	else if (e->eCom.anim->GetCurrentID() == ID::GetID(ID::E_DAMAGE)) {
		if (e->eCom.anim->IsFinish()) {
			e->eCom.anim->Play(ID::E_FALL);
			e->eCom.anim->SetPlaySpeed(1.0f);
			e->eCom.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
		}
		if (!e->eCom.shaker->IsShakeing()) {
			e->eCom.shaker->ShakeFinish();
			e->eCom.shaker->ShakeStart(shakePower, Shaker::MIX_SHAKE, false, -1.2f);
			e->eCom.physics->SetFirction(VECTOR3(3000.0f, 3000.0f, 3000.0f));
		}
	}
	//立ち上がったら棒立ち状態に遷移
	else if (e->eCom.anim->GetCurrentID() == ID::GetID(ID::E_GETUP)) {
		if (e->eCom.anim->IsFinish()) {
			e->eCom.state->ChangeState(ID::E_ANIM_IDOL);
		}
	}
	//落下速度を加速させている
	e->eCom.physics->AddGravity(VECTOR3(0, -GravityAdd, 0));
	if (e->eCom.anim->GetCurrentID() == ID::GetID(ID::E_FALL)) {
		e->eCom.shaker->AddShakePower(shakeAdd);
	}
	
}

void EnemyBlowAway::Draw()
{
}

void EnemyBlowAway::Start()
{
	Enemy* e = GetBase<Enemy>();
	EnemyStateBase::Start();
	pRotation = e->eCom.playerObj->GetTransform()->rotation;
	e->eCom.anim->SetPlaySpeed(2.5f);
	e->eCom.enemy->GetEnemyTransform()->position.y += 10.0f;
	e->eCom.color->setRGB(Color::Rgb(255.0f, 0.0f, 0.0f,255.0f));
	e->eCom.sound->PlaySe(Sound_ID::V_E_DAMAGE3);
}

void EnemyBlowAway::Finish()
{
	Enemy* e = GetBase<Enemy>();
	e->eCom.physics->SetGravity(VECTOR3(0, -150.0f, 0));
	e->eCom.shaker->ShakeFinish();
	e->eCom.physics->SetFirction(VECTOR3(3000.0f, 3000.0f, 3000.0f));
}

void EnemyBlowAway::EnemyBlowAwayInfoSet(EnemyBlowAwayInfo _info)
{
	Enemy* e = GetBase<Enemy>();
	//ダメージを受けた時の話で吹っ飛ばし状態の時にダメージを食らうとまた入ってしまったのでこの処理をいれている。
	if (e->eCom.anim->GetCurrentID() != ID::GetID(ID::E_DAMAGE)) {
		return;
	}
	e->eCom.physics->SetVelocity(VECTOR3(0, 0, _info.backSpeed) * MGetRotY(pRotation.y));
	e->eCom.physics->AddVelocity(VECTOR3(0, _info.UpSpeed, 0), false);
	e->eCom.physics->SetGravity(VECTOR3(0, -_info.gravitySet, 0));
	e->eCom.shaker->ShakeStart(VECTOR3(100,100,100), Shaker::HORIZONAL_SHAKE, true, 0.4f);
	shakeAdd = _info.shakeAdd;
	GravityAdd = _info.gravityAdd;
	shakePower = _info.shakePower;
	e->eCom.physics->SetFirction(VZero);
}
