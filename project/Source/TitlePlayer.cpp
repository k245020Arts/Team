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
#include "TitlePlayerMove.h"

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

	// ステート関連
	playerCom.stateManager->CreateState<TitlePlayerIdol>("_TitleIdol", StateID::PLAYER_WAIT_S);
	playerCom.stateManager->CreateState<TitlePlayerMove>("_TitleMove", StateID::PLAYER_AVOID_S);

	ComponentManager* c = obj->Component();

	playerCom.renderer = c->GetComponent<MeshRenderer>();
	playerCom.meshRenderer2D = c->GetComponent<MeshRenderer2D>();
	playerCom.physics = c->GetComponent<Physics>();
	playerCom.physics->SetGravity(VZero);

    camera = FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<TitleCamera>();

	playerCom.InputManager = FindGameObject<InputManager>();
	playerCom.controller = FindGameObject<ControllerInputManager>();
	playerCom.keyboard = FindGameObject<KeyboardInputManager>();

	VECTOR3 firstPos = VECTOR3(0, 30, 0);
	obj->GetTransform()->position = firstPos;
	playerTransform = obj->GetTransform();

	playerCom.anim = obj->Component()->GetComponent<Animator>();
	playerCom.anim2D = obj->Component()->GetComponent<Anim2D>();
	playerCom.anim->Play(ID::P_ANIM_RUN);
	playerCom.color = obj->Component()->GetComponent<Color>();
	playerCom.shaker = c->GetComponent<Shaker>();

	playerCom.effect = FindGameObject<EffectManager>();
	playerCom.sound = FindGameObject<SoundManager>();

	playerCom.weapon = FindGameObject<WeaponManager>(); //
	playerCom.blur = obj->Component()->GetComponent<MotionBlur>();

	collName = "p_attack";

	using namespace ID;

	playerCom.stateManager->NodeDrawReady();
	playerCom.stateManager->SetComponent<TitlePlayer>(this);

	playerCom.stateManager->StartState(StateID::PLAYER_WAIT_S);
	//3DSoundのベースはプレイヤーに持たせる。
	//playerCom.sound->Base3DSoundObject(obj);
	

	//タイトルシーンの情報はここからとれる。（ボタンを押したときなどの）
	title = GetScene<TitleScene>();
}

void TitlePlayer::RotationChange(VECTOR3 _angle, float _speed)
{
	VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 target = _angle * MGetRotY(camera->GetCameraTransform()->rotation.y);
	float dot = VDot(target.Normalize(), forward.Normalize());	//コサインの値が正面ベクトルとカメラの角度を計算
	//内積を使って補正
	if (dot >= cosf(_speed * DegToRad))
	{
		float inRot = atan2f(target.x, target.z);
		playerTransform->rotation.y = inRot;
	}
	else
	{
		playerTransform->rotation.y = (VDot(right, target) > 0) ? playerTransform->rotation.y + _speed * DegToRad :
		playerTransform->rotation.y - _speed * DegToRad;
	}
}

void TitlePlayer::RotationChange(VECTOR3 _angle)
{
	RotationChange(_angle, 5.0f);
}
