#include "transform.h"
#include "../../../ImGui/imgui.h"

MATRIX Transform::GetPositionMatrix()
{
	return MGetTranslate(position);
}

MATRIX Transform::GetRotationMatrix()
{
	MATRIX mry = MGetRotY(rotation.y);
	MATRIX mrx = MGetRotX(rotation.x);
	MATRIX mrz = MGetRotZ(rotation.z);
	rotMatrix = mrz * mrx * mry;
	return rotMatrix;
}

MATRIX Transform::GetScaleMatrix()
{
	return MGetScale(scale);
}

MATRIX Transform::GetMatrix()
{
	return  GetScaleMatrix() * GetRotationMatrix() * GetPositionMatrix();
}

void Transform::SetParent(Transform* _pare)
{
	if (parent != nullptr) {
		RemoveParent(parent);
	}
	
	parent = _pare;
	if (parent != nullptr) {
		parent->Child(this);
	}
}

Transform Transform::WorldTransform()
{
	VECTOR3 worldPos = position;
	VECTOR3 worldRot = rotation;
	VECTOR3 worldScale = scale;
	if (parent != nullptr) {
		worldPos = position * parent->WorldTransform().GetMatrix();
		worldRot += parent->rotation;
		worldScale *= parent->scale;
	}
	return Transform(worldPos, worldRot, worldScale);
}

void Transform::SetWorld(Transform _transform)
{
	if (parent == nullptr) {
		position = _transform.position;
		rotation = _transform.rotation;
		scale = _transform.scale;
		return;
	}
}

void Transform::ImguiDraw()
{
	ImGui::Separator();
	ImGui::DragFloat3("position",&position.x, 1.0f, 0.0f, 1000);
	ImGui::Separator();
	ImGui::DragFloat3("rotation", &rotation.x, 0.1f, 0.0f, DX_PI_F * 2);
	ImGui::Separator();
	ImGui::DragFloat3("scale", &scale.x, 1.0f, 1.0f, 10.0f);
	ImGui::Separator();
}


void Transform::RemoveChild(Transform* _child)
{
	for (auto itr = child.begin(); itr != child.end(); itr++) {
		if (*itr == _child) {
			_child->RemoveParent(this);
			child.remove(_child);
			return;
		}
	}
}

void Transform::RemoveParent(Transform* transform)
{
	if (parent == nullptr) {
		return;
	}
	if (parent != transform) {
		return;
	}
	//transform->SetParent();
	parent = nullptr;
}
