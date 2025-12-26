#include "CameraEditorGui.h"
#include "../../ImGui/imgui.h"

CameraEditorGui::CameraEditorGui()
{
	camera = nullptr;
}

CameraEditorGui::CameraEditorGui(Camera* _camera)
{
	camera = _camera;
}

CameraEditorGui::~CameraEditorGui()
{
}

void CameraEditorGui::EditorWindow()
{
	ImGui::Begin("editor_mode");
	ImGui::DragFloat3("cameraPosition", &camera->cameraComponent.cameraTransform->position.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("cameraRotation", &camera->cameraComponent.cameraTransform->rotation.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("target", &camera->target.x, 1.0f, 0.0f, 5000.0f);
	if (ImGui::Button("player_position")) {
		camera->cameraComponent.cameraTransform->position = camera->cameraComponent.player.transform->position;
		camera->target = camera->cameraComponent.cameraTransform->position + camera->cameraComponent.player.transform->Forward() * 2000.0f;
	}
	if (ImGui::Button("enemy_transform")) {
		camera->cameraComponent.cameraTransform->position = camera->cameraComponent.target.transform->position;
		camera->target = camera->cameraComponent.cameraTransform->position + camera->cameraComponent.target.transform->Forward() * 2000.0f;
	}
	ImGui::End();
}