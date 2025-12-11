#include "camera.h"
#include "../Component/Transform/Transform.h"
#include "../../ImGui/imgui.h"
#include "../Common/InputManager/ControllerInputManager.h"
#include <cmath>
#include "../Component/ComponentManager.h"
#include "../Component/Shaker/Shaker.h"
#include "../Common/Easing.h"
#include "../Camera/CameraState/CameraStateManager.h"
#include "../Camera/CameraState/FollowCamera.h"
#include "../Camera/CameraState/JustAvoidCamera.h"
#include "../Camera/CameraState/JustAvoidAttackCamera.h"
#include "../Camera/CameraState/JustAvoidAttackHitCamera.h"
#include "../Common/InputManager/InputManager.h"
#include "../Camera/CameraState/FreeCamera.h"
#include "../Enemy/EnemyManager.h"
bool b = false;
Camera::Camera()
{
	cameraComponent.target.transform = nullptr;
	cameraComponent.player.transform = nullptr;
	cameraComponent.cameraTransform = nullptr;
	timeTest = 1.0f;
	normalCamera = true;
	targetChangeButton = 1;
	reap = 0.0f;
	reap = CameraInformation::DEFALUT_RAPE;

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
	direction = EnemyAttackChangeCameraDirection::NONE;
}

Camera::~Camera()
{
	SafeDelete<Transform>(cameraComponent.cameraTransform);

	//delete cameraComponent.state;
}

void Camera::Update()
{
	VECTOR3 a = 0;
	if (CheckHitKey(KEY_INPUT_7) && b)
		b = false;
	else if (CheckHitKey(KEY_INPUT_7) && !b)
		b = true;
	if (b == true)
		a = VECTOR3(0, 4000, 0);
	else
		a = VZero;
	cameraComponent.enemyManager->NearEnemyAlpha(cameraComponent.cameraTransform->position);
	//if (hit) {
	//	hit = false;
	//	//cameraComponent.cameraTransform->position.y += moveAmout;
	//}
	//else {
	//	counter = 0.0f;
	//}
	if (input->KeyInputDown("camera")) {
		if (!rockOn) {
			if (cameraComponent.enemyManager->PlayerDistance(this)) {
				cameraComponent.state->NowChangeState(StateID::FOLLOW_CAMERA_S);
				//rockOn = !rockOn;
			}
			
		}
		else {
			
			cameraComponent.state->NowChangeState(StateID::FREE_CAMERA_S);
			rockOn = !rockOn;
		}
	}
	SetCameraNearFar(10.0f, 100000000.0f);
	SetFogEnable(true);
	SetFogStartEnd(nearFog, farFog);
	SetFogColor(137, 189, 222);
	SetupCamera_Perspective(fov);
	if (debugButton == 2) {
		Transform transform = *obj->GetTransform();
		SetCameraPositionAndTarget_UpVecY(transform.position, VECTOR3(0, 0, 0));
	}
	else if (rockOn) {
		SetCameraPositionAndTarget_UpVecY(cameraComponent.cameraTransform->position+a, target);
	}
	else if (!rockOn) {
		SetCameraPositionAndTarget_UpVecY(cameraComponent.cameraTransform->position, target);
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
	
	
	//DrawSphere3D(target, 50, 1, 0x999999, 0x999999, true);
}

void Camera::Start(BaseObject* _eObj)
{
	SetCameraNearFar(10.0f, 10000.0f);
	SetupCamera_Perspective(DX_PI_F / 3.0f);
	//cameraComponent.target = &target;
	cameraComponent.control = FindGameObject<ControllerInputManager>();
	cameraComponent.shaker = obj->Component()->AddComponent<Shaker>();
	
	cameraComponent.state = obj->Component()->GetComponent<StateManager>();
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

	//stateを登録
	cameraComponent.state->CreateState<FollowCamera>("_FollowCamera", StateID::FOLLOW_CAMERA_S);
	cameraComponent.state->CreateState<JustAvoidCamera>("_JustAvoidCamera", StateID::JUST_AVOID_CAMERA_S);
	cameraComponent.state->CreateState<JustAvoidAttackCamera>("_JustAvoidAttackCamera", StateID::JUST_AVOID_ATTACK_CAMERA_S);
	cameraComponent.state->CreateState<JustAvoidAttackHitCamera>("_JustAvoidAttackHitCamera", StateID::JUST_AVOID_ATTACK_HIT_CAMERA_S);
	cameraComponent.state->CreateState<FreeCamera>("_FreeCamera", StateID::FREE_CAMERA_S);

	cameraComponent.state->NodeDrawReady();
	cameraComponent.target.shaker = _obj->Component()->GetComponent<Shaker>();

	cameraComponent.state->SetComponent<Camera>(this);
	cameraComponent.state->StartState(StateID::FREE_CAMERA_S);
	//CameraRotationSet();
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

void Camera::ChangeStateCamera(StateID::State_ID _id)
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

	// ======== 2. カメラの向くべき方向 ========
	//VECTOR3 p = VECTOR3(0, 500, -1500);
	//cameraComponent.cameraTransform->position = cameraComponent.player.transform->position + p;
	//VECTOR3 dir = cameraComponent.target.transform->position - cameraComponent.cameraTransform->position;
	//dir = dir.Normalize();
	//VECTOR3 up = VGet(0, 1, 0);

	//MATRIX lookM = MyLookAt(cameraComponent.cameraTransform->position, cameraComponent.cameraTransform->position + dir, up);

	//Quaternion targetQ = MToQ(lookM);

	//// ======== 3. 今の回転へ自然に補間 (SLERP) ========
	//float t = 1.0f - std::pow(0.0001f, Time::DeltaTimeRate() * 6.0f);
	//cameraQuaternion = QSlerp(cameraQuaternion, targetQ, t);

	//// ======== 4. DxLibカメラに反映 ========
	//target = cameraComponent.cameraTransform->position + QApply(cameraQuaternion, VGet(0, 0, 1));  // クォータニオン回転後の前方ベクトル
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
	
	/*if (fabs(beforePos - cameraComponent.cameraTransform->position.y) <= 1.5f) {
		cameraComponent.cameraTransform->position.y = beforePos;
	}

	beforePos = cameraComponent.cameraTransform->position.y;*/
}

void Camera::CollsionPosHit(VECTOR3 norm, float size, VECTOR3 groundPos)
{
	hitPos = groundPos;
	hitNormal = norm;
	hitDist = size;
	hit = true;
}

void Camera::PushCamera(VECTOR3 norm, float size, VECTOR3 groundPos)
{
	//// カメラの現在位置
	//VECTOR3 camPos = cameraComponent.cameraTransform->position;

	//// 最低でも保ちたい距離（プレイヤーとカメラの離隔距離）
	//const float MIN_DIST = 150.0f;  // 好みで調整

	//// 衝突点 → カメラの方向（角度が重要なので正規化する）
	//VECTOR3 toCam = (camPos - norm);
	//float dist = toCam.Size();

	//// 遠いなら押し返す必要なし
	//if (dist >= MIN_DIST)
	//	return;

	//toCam = toCam.Normalize();

	//// 足りていない距離（押し返す量）
	//float pushAmount =  dist;

	//// 角度を維持したまま押し返す（法線ではなく toCam 方向！）
	//VECTOR3 correctedPos = camPos + toCam * pushAmount;

	//// ガタつかないように Lerp で補正
	//cameraComponent.cameraTransform->position =
	//	Easing::Lerp(camPos, correctedPos, 1.0f);

	//currentDistance.z = (defalutDistance.z, 100.0f, 1.0f);
	float offset = 5.0f; 
	float newDist = max(0, size - offset);
	counter = counter > 1.0f ? 1.0f : counter + Time::DeltaTimeRate() * 3.0f;
	float rate = counter / 1.0f;
	//currentDistance.z = Easing::Lerp(defalutDistance.z, 1000.0f, rate);
	hit = true;
	cameraComponent.cameraTransform->position = VECTOR3(cameraComponent.cameraTransform->position.x ,groundPos.y + 280.0f, cameraComponent.cameraTransform->position.z);
}

void Camera::AttackEnemyFovChange(Transform* _targetTransform)
{
	direction = cameraComponent.enemyManager->BossAttackCamera(this, *_targetTransform);
	targetEnemyTransform = _targetTransform;
}

bool Camera::IsFovIn(const Transform& _targetTransform, float maxFov)
{
	VECTOR3 dir = target - cameraComponent.cameraTransform->position;
	dir = dir.Normalize();

	VECTOR3 target = _targetTransform.position - cameraComponent.player.obj->GetTransform()->position;
	float dist = target.Size();
	if (VDot(dir.Normalize(), target.Normalize()) < cosf(maxFov)) {
		return false;
	}
	else {
		return true;
	}
}
