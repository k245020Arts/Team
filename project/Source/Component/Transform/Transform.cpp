#include "transform.h"
#include "../../../ImGui/imgui.h"
#include "Quaternion.h"

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

	//Quaternion qLocal = Quaternion::FromEuler(rotation);

	//VECTOR3 worldPos = position;
	//Quaternion qWorld = qLocal;
	//VECTOR3 worldScale = scale;

	//if (parent != nullptr) {
	//	// 親のワールドを一度だけ取得（再帰）
	//	Transform pWorld = parent->WorldTransform();

	//	// 親の回転をクォータニオン化して合成（親 * local）
	//	Quaternion qParent = Quaternion::FromEuler(pWorld.rotation);
	//	qWorld = qParent * qLocal; // 親の回転が先に来る

	//	// 位置は親のワールド行列で変換（既存の VECTOR3 * MATRIX 実装を使用）
	//	worldPos = position * parent->WorldTransform().GetMatrix();

	//	// スケールは要素ごとの乗算
	//	worldScale *= pWorld.scale;
	//}

	//// 合成したクォータニオンをオイラーに戻す（表示や既存コード互換のため）
	//VECTOR3 worldRot = qWorld.ToEuler();

	//return Transform(worldPos, worldRot, worldScale);
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

	if (ImGui::Button("posCopy")) {
		char buf[90];
		sprintfDx(buf,"VECTOR3(%.2f,%.2f,%.2f)", position.x, position.y, position.z);
		std::string copyData = buf;
		SetClipboardText(copyData.c_str());
	}
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
