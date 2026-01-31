#include "TitleCamera.h"
#include "Component/Transform/Transform.h"
#include "../ImGui/imgui.h"
#include "Common/InputManager/ControllerInputManager.h"
#include <cmath>
#include "Component/ComponentManager.h"
#include "Component/Shaker/Shaker.h"
#include "Common/Easing.h"
#include "TitleScene.h"

TitleCamera::TitleCamera()
{
	reap = 0.0f;
	reap = CameraInformation::DEFALUT_RAPE;

	defalutDistance = VECTOR3(0.0f, 0.0f, -1500.0f);
	currentDistance = defalutDistance;
	
	debugId = 33;
	tag = Function::GetClassNameC<TitleCamera>();

	fov = 60.0f * DegToRad;
}

TitleCamera::~TitleCamera()
{
	delete cameraComponent.cameraTransform;
	cameraComponent.cameraTransform = nullptr;
}

void TitleCamera::Update()
{
	target = cameraComponent.player.transform->position;
	CameraRotationSet();
	Follow();
	
}

void TitleCamera::Draw()
{

	SetCameraNearFar(10.0f, 5000000.0f);
	/*SetFogEnable(true);
	SetFogStartEnd(nearFog, farFog);
	SetFogColor(137, 189, 222);*/
	SetupCamera_Perspective(fov);

	if (debugButton == 1) {
		Transform transform = *obj->GetTransform();
		VECTOR3 forward = VGet(0, 0, 1); // 仮でもいい
		SetCameraPositionAndTarget_UpVecY(transform.position, transform.position + forward * 1000.0f);
	}
	else {
		SetCameraPositionAndTarget_UpVecY(cameraComponent.cameraTransform->position, target);
	}


}

void TitleCamera::ImguiDraw()
{
	//使用するTransformが違う
	//normalCameraの時はcameraComponent.CameraTransformを使用
	//DebugCameraの時は通常のTransformの使用
	ImGui::RadioButton("normalCamera", &debugButton, 0);
	ImGui::RadioButton("DebugCamera", &debugButton, 1);
	ImGui::DragFloat3("cameraPosition", &cameraComponent.cameraTransform->position.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("cameraRotation", &cameraComponent.cameraTransform->rotation.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("target", &target.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("DebugTarget", &diffTarget.x, 1.0f, -100000.0f, 10000.0f);
}

void TitleCamera::Start(BaseObject* _eObj)
{
}

void TitleCamera::PlayerSet(BaseObject* _pObj)
{
	cameraComponent.player.obj = _pObj;
	cameraComponent.player.transform = _pObj->GetTransform();
	//playerに追従させたいのでカメラのトランスフォームはプレイヤーを軸にする。
	cameraComponent.cameraTransform = new Transform(*cameraComponent.player.transform);
	cameraComponent.cameraTransform->rotation.x = 30.0f * DegToRad;
	cameraComponent.player.shaker = _pObj->Component()->GetComponent<Shaker>();

	cameraComponent.target.shaker = _pObj->Component()->GetComponent<Shaker>();

	cameraComponent.shaker = obj->Component()->AddComponent<Shaker>();

	//タイトルシーンの情報はここからとれる。（ボタンを押したときなどの）
	title = GetScene<TitleScene>();

}