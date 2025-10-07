#include "Object3D.h"
#include "transform.h"
#include "ComponentManager.h"
#include "../ImGui/imgui.h"
#include "color.h"
#include "Hierarchy.h"
#include "TransitorManager.h"
#include "../Library/MemoryCount.h"
#include "Debug.h"

Object3D::Object3D()
{
	
}

Object3D::~Object3D()
{
	
	/*if (transform != nullptr) {
		delete transform;
	}*/
}

void Object3D::Update()
{
	BaseObject::Update();
	
}

void Object3D::Draw()
{
	BaseObject::Draw();
}

void Object3D::Init(const VECTOR3 _pos, const VECTOR3 _rotate, const VECTOR3 _scale,std::string name)
{
	BaseObject::Init(name);
	transform = Component()->AddComponent<Transform>();
	transform->position = _pos;
	transform->rotation = _rotate;
	transform->scale = _scale;
}

void Object3D::Init(Transform _transform, std::string name)
{
	Init(_transform.position, _transform.rotation, _transform.scale, name);
}
