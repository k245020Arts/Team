#include "EnemyBlowAway.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/Animator/Animator.h"
#include "../Enemy.h"
#include "../../../Component/Shaker/Shaker.h"
#include "EnemyStateManager.h"
#include "../../../Component/Transform/transform.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/InputManager/ControllerInputManager.h"
#include "../../../Component/Color/color.h"
#include "../../../Common/Sound/SoundManager.h"

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
	if (e->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(ID::E_FALL)) {
		if (e->enemyBaseComponent.enemy->GetEnemyTransform()->position.y <= 0.0f) {
			e->enemyBaseComponent.anim->Play(ID::E_GETUP);
			e->enemyBaseComponent.shaker->ShakeFinish();
			e->enemyBaseComponent.camera->CameraShake(VECTOR3(100, 150, 100), Shaker::HEIGHT_SHAKE, false , 0.5f);
			e->enemyBaseComponent.control->ControlVibrationStartFrame(200, 30);
			e->enemyBaseComponent.sound->PlaySe(Sound_ID::ENEMY_FALL);
			e->enemyBaseComponent.sound->PlaySe(Sound_ID::V_E_DAMAGE4);
		}
	}
	//ダメージの赤点滅
	else if (e->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(ID::E_DAMAGE)) {
		if (e->enemyBaseComponent.anim->IsFinish()) {
			e->enemyBaseComponent.anim->Play(ID::E_FALL);
			e->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
			e->enemyBaseComponent.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
		}
		if (!e->enemyBaseComponent.shaker->IsShakeing()) {
			e->enemyBaseComponent.shaker->ShakeFinish();
			e->enemyBaseComponent.shaker->ShakeStart(shakePower, Shaker::MIX_SHAKE, false, -1.2f);
			e->enemyBaseComponent.physics->SetFirction(VECTOR3(3000.0f, 3000.0f, 3000.0f));
		}
	}
	//立ち上がったら棒立ち状態に遷移
	else if (e->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(ID::E_GETUP)) {
		if (e->enemyBaseComponent.anim->IsFinish()) {
			e->enemyBaseComponent.state->ChangeState(ID::E_ANIM_IDOL);
		}
	}
	//落下速度を加速させている
	e->enemyBaseComponent.physics->AddGravity(VECTOR3(0, -GravityAdd, 0));
	if (e->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(ID::E_FALL)) {
		e->enemyBaseComponent.shaker->AddShakePower(shakeAdd);
	}
	
}

void EnemyBlowAway::Draw()
{
}

void EnemyBlowAway::Start()
{
	Enemy* e = GetBase<Enemy>();
	EnemyStateBase::Start();
	pRotation = e->enemyBaseComponent.playerObj->GetTransform()->rotation;
	e->enemyBaseComponent.anim->SetPlaySpeed(2.5f);
	e->enemyBaseComponent.enemy->GetEnemyTransform()->position.y += 10.0f;
	e->enemyBaseComponent.color->setRGB(Color::Rgb(255.0f, 0.0f, 0.0f,255.0f));
	e->enemyBaseComponent.sound->PlaySe(Sound_ID::V_E_DAMAGE3);
}

void EnemyBlowAway::Finish()
{
	Enemy* e = GetBase<Enemy>();
	e->enemyBaseComponent.physics->SetGravity(VECTOR3(0, -150.0f, 0));
	e->enemyBaseComponent.shaker->ShakeFinish();
	e->enemyBaseComponent.physics->SetFirction(VECTOR3(3000.0f, 3000.0f, 3000.0f));
}

void EnemyBlowAway::EnemyBlowAwayInfoSet(EnemyBlowAwayInfo _info)
{
	Enemy* e = GetBase<Enemy>();
	//ダメージを受けた時の話で吹っ飛ばし状態の時にダメージを食らうとまた入ってしまったのでこの処理をいれている。
	if (e->enemyBaseComponent.anim->GetCurrentID() != ID::GetID(ID::E_DAMAGE)) {
		return;
	}
	e->enemyBaseComponent.physics->SetVelocity(VECTOR3(0, 0, _info.backSpeed) * MGetRotY(pRotation.y));
	e->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, _info.UpSpeed, 0), false);
	e->enemyBaseComponent.physics->SetGravity(VECTOR3(0, -_info.gravitySet, 0));
	e->enemyBaseComponent.shaker->ShakeStart(VECTOR3(100,100,100), Shaker::HORIZONAL_SHAKE, true, 0.4f);
	shakeAdd = _info.shakeAdd;
	GravityAdd = _info.gravityAdd;
	shakePower = _info.shakePower;
	e->enemyBaseComponent.physics->SetFirction(VZero);
}
