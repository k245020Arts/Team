#include "MeshRenderer.h"
#include "transform.h"
#include "../ImGui/imgui.h"
#include "ComponentManager.h"
#include "Shaker.h"

MeshRenderer::MeshRenderer()
{
	hModel = -1;
	draw = true;
	shaker = nullptr;
	debugId = 6;
	tag = Function::GetClassNameC<MeshRenderer>();
	angle = 0.0f;
	color = nullptr;
	meshNum = 0;
	worldTransform = true;
}

MeshRenderer::~MeshRenderer()
{
	MV1DeleteModel(hModel);
}

void MeshRenderer::Update()
{

}

void MeshRenderer::Draw()
{
	if (!draw) {
		return;
	}
	if (hModel < 0) {
		return;
	}

	//描画専用のトランスフォームを作る
	Transform tra = *obj->GetTransform();
	if (shaker != nullptr) {
		//ヒットストップなどでシェイクしているときは描画用のトランスフォームだけを動かしている
		tra.position += shaker->GetShakePower();
	}
	if (worldTransform) {
		MV1SetMatrix(hModel, tra.WorldTransform().GetMatrix());
	}
	else {
		MV1SetMatrix(hModel, tra.GetMatrix());
	}
	
	MV1SetDifColorScale(hModel, color->GetColorF());
	MV1DrawModel(hModel);
}

void MeshRenderer::ModelHandle(int _hHandle)
{
	hModel = _hHandle;
	color = obj->Component()->GetComponent<Color>();
	shaker = obj->Component()->GetComponent<Shaker>();
}

void MeshRenderer::ImguiDraw()
{
	ImGui::DragInt("mesh", &meshNum, 1.0f, 0, 32);
	ImGui::DragFloat("ang", &angle, 1.0f, 0, 360);
	if (ImGui::Button("button")) {
		
		RotationMesh(meshNum, angle * DegToRad);
			
	}
	if (ImGui::Button("reset")) {
		MV1ResetFrameUserLocalMatrix(hModel,meshNum);
	}
	if (ImGui::Button("+")) {
		meshNum++;
	}
	if(ImGui::Button("noDraw")) {
		draw = !draw;
	}
}

void MeshRenderer::RotationMesh(int _meshNum, float _angle)
{
	int a = MV1SetFrameUserLocalMatrix(hModel, _meshNum, MGetRotY(_angle));
}
