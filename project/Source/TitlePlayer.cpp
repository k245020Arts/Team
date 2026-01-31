#include "TitlePlayer.h"
#include "TitleCamera.h"
#include "State/StateManager.h"
#include "Player/PlayerState/PlayerStateManager.h"
#include "Component/MeshRenderer/MeshRenderer.h"
#include "Component/MeshRenderer2D/MeshRenderer2D.h"
#include "Component/Animator/Anim2D.h"
#include "Component/Physics/Physics.h"
#include "Camera/Camera.h"
#include "Common/InputManager/InputManager.h"
#include "Common/InputManager/controllerInputManager.h"
#include "Common/InputManager/keyboardInputManager.h"
#include "../ImGui/imgui.h"
#include "Component/Transform/Transform.h"
#include "Component/Animator/Animator.h"
#include "Camera/cameraManager.h"
#include "Player/PlayerState/PlayerStateBase.h"
#include "Component/ComponentManager.h"
#include "Component/Shaker/Shaker.h"
#include "Common/Sound/SoundManager.h"
#include "Common/Effect/EffectManager.h"
#include "Weapon/WeaponManager.h"
#include "Component/MotionBlur/MotionBlur.h"
#include "Common/LoadManager.h"
#include "Common/Transitor/FadeTransitor.h"
#include "Common/Easing.h"
#include "TitleScene.h"
#include "TitlePlayerIdol.h"

TitlePlayer::TitlePlayer()
{
	debugId = 34;
	tag = Function::GetClassNameC<TitlePlayer>();
}

TitlePlayer::~TitlePlayer()
{
}

void TitlePlayer::Update()
{
	//Move(1500.0f, 3000.0f);
}

void TitlePlayer::Draw()
{
}

void TitlePlayer::Start(Object3D* _obj) {
	//初期化の値を設定
	//必要なコンポーネントを付けている。
	//使いたいコンポーネントがあればplayerInformation.hに情報が書いてあるのでそれを見てほしい
	obj = _obj;
	playerCom.stateManager = obj->Component()->GetComponent<StateManager>();

	playerCom.stateManager->CreateState<TitlePlayerIdol>("_TitleIdol", StateID::PLAYER_WAIT_S);

	ComponentManager* c = obj->Component();

	playerCom.renderer = c->GetComponent<MeshRenderer>();
	playerCom.meshRenderer2D = c->GetComponent<MeshRenderer2D>();
	playerCom.physics = c->GetComponent<Physics>();

    camera = FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<TitleCamera>();

	//playerCom.InputManager = FindGameObject<ImputManager>();
	playerCom.controller = FindGameObject<ControllerInputManager>();
	playerCom.keyboard = FindGameObject<KeyboardInputManager>();
	/*playerCom.InputManager = FindGameObject<InputManager>();*/

	playerTransform = obj->GetTransform();

	playerCom.anim = obj->Component()->GetComponent<Animator>();
	playerCom.anim2D = obj->Component()->GetComponent<Anim2D>();
	playerCom.anim->Play(ID::P_ANIM_RUN);
	playerCom.color = obj->Component()->GetComponent<Color>();
	//playerCom.targetObj = FindGameObjectWithTag<Object3D>("ENEMY");
	playerCom.targetObj = nullptr;
	playerCom.hitObj = nullptr;
	playerCom.shaker = c->GetComponent<Shaker>();

	playerCom.effect = FindGameObject<EffectManager>();
	playerCom.sound = FindGameObject<SoundManager>();

	playerCom.weapon = FindGameObject<WeaponManager>();
	playerCom.blur = obj->Component()->GetComponent<MotionBlur>();

	/*playerCom.gameManager = FindGameObject<GameManager>();
	playerCom.enemyManager = FindGameObject<EnemyManager>();*/

	//playerCom.specialAttackButton = FindGameObjectWithTag<Object2D>("XButton")->Component()->GetComponent<ButtonUI>();

	/*avoidStart = false;
	justAvoidCanCounter = 0.0f;*/
	attackColl = nullptr;
	collName = "p_attack";
	//playerCom.physics->SetVelocity(VECTOR3(10.0f, 5.0f, 0.0f));

	//physics->SetInterect(VECTOR3(5.0f, -1.0f, 0.0f),0.1);
	using namespace ID;
	//ステートのセット


	playerCom.stateManager->NodeDrawReady();
	playerCom.stateManager->SetComponent<TitlePlayer>(this);

	playerCom.stateManager->StartState(StateID::PLAYER_WAIT_S);
	/*redCounter = 0.0f;*/
	//3DSoundのベースはプレイヤーに持たせる。
	//playerCom.sound->Base3DSoundObject(obj);
	

	//タイトルシーンの情報はここからとれる。（ボタンを押したときなどの）
	title = GetScene<TitleScene>();
}

void TitlePlayer::Move(float _speed, float _speedMax)
{
	//if (CheckHitKey(KEY_INPUT_RIGHT)) {
	//	//VECTOR3 power = com.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
	//	//if (power.Size() <= 1.0f) {
	//	playerCom.physics->AddVelocity(VECTOR3(50.0f, 0.0f, 0.0f), false);
	//	//}

	//}
	//PlayerStickInput();
	if (CheckHitKey(KEY_INPUT_A)) {
		//VECTOR3 power = com.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
		//if (power.Size() >= -1.0f) {
		walkAngle.x = -1.0f;
		//}
	}
	else if (CheckHitKey(KEY_INPUT_W)) {
		walkAngle.z = 1.0f;
	}
	else if (CheckHitKey(KEY_INPUT_S)) {
		walkAngle.z = -1.0f;
	}
	else if (CheckHitKey(KEY_INPUT_D)) {
		walkAngle.x = 1.0f;
	}


	std::shared_ptr<PlayerStateBase> pB = playerCom.stateManager->GetState<PlayerStateBase>();

	StickDirections stick = playerCom.controller->GetStickKnockingReverce(0.6f, 8).leftStick;
	StickDirections nowStick = playerCom.controller->GetStickKnocking(0.6f, 1).leftStick;

	//スティックの傾きの量が少なかったら移動しない
	if ((fabs(walkAngle.x) >= 0.3f || fabs(walkAngle.z) >= 0.3f) && hp > 0.0f) {
		//回転処理
		RotationChange(walkAngle, 12.0f);

		VECTOR3 dir = VZero;
		dir.x = walkAngle.x * 1.0f * _speed;
		dir.z = walkAngle.z * 1.0f * _speed;
		dir.y *= 0.0f;
		dir = dir * MGetRotY(playerCom.camera->GetCameraTransform()->rotation.y);
		playerCom.physics->AddVelocity(dir, false);
		VECTOR3 moveVelo;
		moveVelo = playerCom.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);

		float max = _speedMax;
		//size = moveVelo.SquareSize();

		//最大速度までいったらスピードマックスに補正
		if (moveVelo.SquareSize() >= max * max) {
			moveVelo = moveVelo.Normalize() * _speedMax;
			moveVelo.y = playerCom.physics->GetVelocity().y;
			playerCom.physics->SetVelocity(moveVelo);
		}
		//playerCom.stateManager->ChangeState(StateID::PLAYER_WALK_S);
		//アニメーションのスピードを傾き方で測定
		playerCom.anim->SetPlaySpeed(walkAngle.Size());


		/*if (nowStick == S_NO_DIRECTION || stick == S_NO_DIRECTION) {
			return;
		}
		if (nowStick == stick) {
			playerCom.stateManager->ChangeState(StateID::PLAYER_TURN_S);
		}*/
	}
	else {
		//playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}

}


void TitlePlayer::RotationChange(VECTOR3 _angle, float _speed)
{
	VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 target = _angle * MGetRotY(camera->GetCameraTransform()->rotation.y);
	float dot = VDot(target.Normalize(), forward.Normalize());	//コサインの値が正面ベクトルとカメラの角度を計算
	//内積を使って補正
	if (dot >= cosf(_speed * DegToRad)) {
		float inRot = atan2f(target.x, target.z);
		playerTransform->rotation.y = inRot;
	}
	else {
		playerTransform->rotation.y = (VDot(right, target) > 0) ? playerTransform->rotation.y + _speed * DegToRad :
			playerTransform->rotation.y - _speed * DegToRad;
	}
}

void TitlePlayer::RotationChange(VECTOR3 _angle)
{
	RotationChange(_angle, 5.0f);
}
