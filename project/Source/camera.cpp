#include "camera.h"
#include "transform.h"
#include "../ImGui/imgui.h"
#include "controllerInputManager.h"
#include <cmath>
#include "ComponentManager.h"
#include "Shaker.h"
#include "Easing.h"
#include "cameraStateManager.h"
#include "FollowCamera.h"
#include "JustAvoidCamera.h"
#include "JustAvoidAttackCamera.h"
#include "JustAvoidAttackHitCamera.h"
#include "inputManager.h"
#include "FreeCamera.h"
#include "EnemyManager.h"

Camera::Camera()
{
	cameraComponent.target.transform = nullptr;
	cameraComponent.player.transform = nullptr;
	cameraComponent.cameraTransform = nullptr;
	timeTest = 1.0f;
	normalCamera = true;
	targetChangeButton = 1;
	reap = 0.0f;
	reap = 0.2f;

	defalutDistance = VECTOR3(0.0f,0.0f, -1500.0f);
	currentDistance = defalutDistance;
	debugButton = 0;
	debugId = 12;
	tag = Function::GetClassNameC<Camera>();
	fov = 60.0f * DegToRad;

	input = FindGameObject<InputManager>();
	rockOn = false;
	beforePos = 0.0f;
	nearFog = 10000.0f;
	farFog = 1800000.0f;
}

Camera::~Camera()
{
	delete cameraComponent.cameraTransform;
	//delete cameraComponent.state;
}

void Camera::Update()
{
	if (input->KeyInputDown("camera")) {
		if (!rockOn) {
			if (cameraComponent.enemyManager->PlayerDistance(this)) {
				cameraComponent.state->NowChangeState(ID::C_FOLLOW);
				//rockOn = !rockOn;
			}
			
		}
		else {
			
			cameraComponent.state->NowChangeState(ID::C_FREE);
			rockOn = !rockOn;
		}
	}
	
}

void Camera::Draw()
{
	//カメラがフェード中も止まってしまうのが嫌だったので一時的にdrawのところに逃がしている
	if (rockOn) {
		//cameraComponent.state->Update();
	}
	else {
		
	}
	SetCameraNearFar(10.0f, 100000000.0f);
	SetFogEnable(true);
	SetFogStartEnd(nearFog,farFog);
	SetFogColor(137, 189, 222);
	SetupCamera_Perspective(fov);
	if (debugButton == 2) {
		Transform transform = *obj->GetTransform();
		SetCameraPositionAndTarget_UpVecY(transform.position, VECTOR3(0, 0, 0));
	}
	else if (rockOn) {
		SetCameraPositionAndTarget_UpVecY(cameraComponent.cameraTransform->position,target);
	}
	else if (!rockOn) {
		SetCameraPositionAndTarget_UpVecY(cameraComponent.cameraTransform->position,target);
	}
	
	//DrawSphere3D(target, 50, 1, 0x999999, 0x999999, true);
}

void Camera::Start(BaseObject* _eObj)
{
	SetCameraNearFar(10.0f, 10000.0f);
	SetupCamera_Perspective(DX_PI_F / 3.0f);
	//cameraComponent.target = &target;
	cameraComponent.control = FindGameObject<ControllerInputManager>();
	cameraComponent.shaker = obj->Component()->AddComponent<Shaker>();
	
	cameraComponent.state = obj->Component()->AddComponent<StateManager>();

}

void Camera::ImguiDraw()
{
	//cameraComponent.state->StateNodeDraw();
	ImGui::RadioButton("normalCamera",&debugButton ,0);
	ImGui::RadioButton("FreeCamera", &debugButton, 1);
	ImGui::RadioButton("DiffTarget", &debugButton, 2);
	ImGui::DragFloat3("cameraPosition", &cameraComponent.cameraTransform->position.x,1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("cameraRotation", &cameraComponent.cameraTransform->rotation.x,1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("target", &target.x,1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat("time", &timeTest,1.0f,0.0f, 2.0f);
	//ImGui::DragFloat3("distance", cameraComponent.currentDistance.x,5.0f,-1500.0f, 1500.0f);
	ImGui::RadioButton("targetPlayer", &targetChangeButton, 0);
	ImGui::RadioButton("targetEnemy", &targetChangeButton, 1);
	ImGui::DragFloat("fov", &fov, 0.1f, 0.0f, 360.0f * DegToRad);
	ImGui::InputFloat("nearFog", &nearFog);
	ImGui::InputFloat("farFog", &farFog);

}

void Camera::PlayerSet(BaseObject* _obj)
{
	cameraComponent.player.obj = _obj;
	cameraComponent.camera = this;
	cameraComponent.player.transform = _obj->GetTransform();
	cameraComponent.cameraTransform =  new Transform(*cameraComponent.player.transform);
	cameraComponent.cameraTransform->rotation.x = 30.0f * DegToRad;
	cameraComponent.player.shaker = _obj->Component()->GetComponent<Shaker>();
	cameraComponent.enemyManager = FindGameObject<EnemyManager>();

	using namespace ID;

	//stateを登録
	cameraComponent.state->CreateState<FollowCamera>(GetID(C_FOLLOW));
	cameraComponent.state->CreateState<JustAvoidCamera>(GetID(P_ANIM_JUST_AVOID));
	cameraComponent.state->CreateState<JustAvoidAttackCamera>(GetID(C_AVOID_ATTACK));
	cameraComponent.state->CreateState<JustAvoidAttackHitCamera>(GetID(C_HIT));
	cameraComponent.state->CreateState<FreeCamera>(GetID(C_FREE));

	cameraComponent.state->NodeDrawReady();
	cameraComponent.target.shaker = _obj->Component()->GetComponent<Shaker>();

	cameraComponent.state->SetComponent<Camera>(this);
	cameraComponent.state->StartState(C_FREE);
}

void Camera::CameraShake(VECTOR3 _power,Shaker::ShakePattern _pattern,bool _stop,float _second)
{
	//カメラを震わせるときはこれを使う
	cameraComponent.shaker->ShakeStart(_power, _pattern, _stop, _second, cameraComponent.cameraTransform);
}

void Camera::CameraShakeStop()
{
	cameraComponent.shaker->ShakeFinish();
}

void Camera::TargetSet(BaseObject* _obj)
{
	cameraComponent.target.obj = _obj;
	cameraComponent.target.transform = cameraComponent.target.obj->GetTransform();
	rockOn = true;
}

void Camera::TargetEnemySet()
{
	target = cameraComponent.target.transform->position;
}

void Camera::CameraLeapSet(float _rape)
{
	//カメラ追従していくスピードをセットしている
	reap = _rape;
}

void Camera::ChangeStateCamera(ID::IDType _id)
{
	cameraComponent.state->ChangeState(_id);
}

void Camera::CameraRotationSet()
{
	//敵に追従したカメラの角度のセットをしている関数
	VECTOR3 dir;

	dir = target - cameraComponent.cameraTransform->position;

	dir = dir.Normalize();
	float angle = atan2f(dir.x, dir.z);
	cameraComponent.cameraTransform->rotation.y = angle;
}

void Camera::Follow()
{
	//追従処理
	VECTOR3 offset = currentDistance * cameraComponent.cameraTransform->GetRotationMatrix();
	VECTOR3 desiredCamPos;
	desiredCamPos = cameraComponent.player.transform->position + offset;
	//カメラのポジションをイージングの割合でセットしている
	cameraComponent.cameraTransform->position = Easing::Lerp(cameraComponent.cameraTransform->position, desiredCamPos, reap);
	//そのあとにカメラshakeをかける
	cameraComponent.cameraTransform->position += cameraComponent.shaker->GetShakePower();
	if (fabs(beforePos - cameraComponent.cameraTransform->position.y) <= 1.5f) {
		cameraComponent.cameraTransform->position.y = beforePos;
	}

	beforePos = cameraComponent.cameraTransform->position.y;
}
