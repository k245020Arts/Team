#pragma once
#include "../Library/myDxLib.h"
#include "Component.h"

class Transform : public Component
{
public:
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;
	VECTOR3 Forward() { return VECTOR3(0, 0, 1) * rotMatrix; }
	Transform() {
		position = VZero;
		rotation = VZero;
		scale = VECTOR3(1, 1, 1);
		parent = nullptr;
		rotMatrix = MGetIdent();
		positionMatrix = MGetIdent();
		scaleMatrix = MGetIdent();
		debugId = 11;
		tag = Function::GetClassNameC<Transform>();
	}
	Transform(const VECTOR3 _pos, const VECTOR3 _rotate, const VECTOR3 _scale) {
		position = _pos;
		rotation = _rotate;
		scale = _scale;
		parent = nullptr;
		rotMatrix = MGetIdent();
		positionMatrix = MGetIdent();
		scaleMatrix = MGetIdent();
		debugId = 11;
		tag = Function::GetClassNameC<Transform>();
	}
	~Transform(){
		if (parent != nullptr) {
			parent->RemoveChild(this);
		}
		for (auto itr = child.begin(); itr != child.end(); itr++) {
			(*itr)->RemoveParent(this);
		}
	}

	MATRIX GetPositionMatrix();
	MATRIX GetRotationMatrix();
	MATRIX GetScaleMatrix();

	MATRIX GetMatrix();

	//子が親を追加したいとき
	void SetParent(Transform* _pare);
	//親が子を追加したいとき
	void AddChild(Transform* _child) { 
		if (_child->parent) {
			_child->parent->RemoveChild(_child); // 元の親から外す
		}
		_child->parent = this;
		child.emplace_back(_child);
	}
	//子のポジションのワールド座標を取り出す関数。
	Transform WorldTransform();
	
	void SetWorld(Transform _transform);
	void ImguiDraw()override;

	void RemoveChild(Transform* _child);

private:

	MATRIX rotMatrix;
	MATRIX positionMatrix;
	MATRIX scaleMatrix;

	Transform* parent;
	std::list<Transform*> child;

	void RemoveParent(Transform* transform);

	void Child(Transform* _child){ child.emplace_back(_child); }
};