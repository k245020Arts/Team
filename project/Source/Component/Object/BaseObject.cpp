#include "BaseObject.h"
#include "../Transform/Transform.h"
#include "../ComponentManager.h"
#include "../../../ImGui/imgui.h"
#include "../Color/Color.h"
#include "../Hierarchy/Hierarchy.h"
#include "../../Common/Transitor/TransitorManager.h"
#include "../../Common/Memory/MemoryCount.h"
#include "../../Common/Debug/Debug.h"
#include "../../PlayScene.h"

int BaseObject::idPlus = 0;

BaseObject::BaseObject()
{
	objectTimeRate = 1.0f;
	baseTimeSpeed = 1.0f;
	componentManager = nullptr;
	transform = nullptr;
	transitor = nullptr;
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	id = idPlus++;
	parent = nullptr;
	transformParent = true;
	sleep = false;
}

BaseObject::~BaseObject()
{
	if (GetTag() == "PLAYER") {
		int a = 0;
	}
	delete componentManager;
	FindGameObject<Hierachy>()->RemoveHierachy(this);
	RemoveParent();
	DeleteAllChildren();
}

void BaseObject::Update()
{
	//フェード中なら動かさない
	/*if (!transitor->IsTransitor()) {
		return;
	}*/
	if (sleep) {
		return;
	}
	if (GetTag() == "STAGE") {
		int a = 0;
	}
	PlayScene* playscene = GetScene<PlayScene>();
	if (GetTag() != "CAMERA_OBJ") {
		if (playscene->GetMode() == PlayScene::CAMERA_EDITOR) {
			return;
		}
	}
	//コンポーネントで回している
	componentManager->Update();
	//子供のオブジェクトをまわす
	for (BaseObject* child : children) {
		child->Update();
	}

}

void BaseObject::Draw()
{
	//描画
	componentManager->Draw();
	for (BaseObject* child : children) {
		child->Draw();
	}
}

void BaseObject::Init(std::string name)
{
	componentManager = new ComponentManager();

	componentManager->Init(this);
	SetTag(name);
	//デバックウィンドウはここで登録している
	FindGameObject<Hierachy>()->Start(name, this);
	transitor = FindGameObject<TransitorManager>();
}

void BaseObject::AddChild(BaseObject* child)
{
	AddChild(child, true);
}

void BaseObject::AddChild(BaseObject* child, bool _transformParent)
{
	transformParent = _transformParent;
	if (transformParent) {
		if (child->parent) {
			child->parent->RemoveChild(child); // 元の親から外す
		}
		transform->AddChild(child->GetTransform());
	}
	child->parent = this;
	ObjectManager::Pop(dynamic_cast<GameObject*>(child));
	children.emplace_back(child);
}

void BaseObject::RemoveChild(BaseObject* child) {
	for (auto itr = children.begin(); itr != children.end();) {
		if (*itr == child) {
			BaseObject* obj = *itr;
			if (transformParent && transform != nullptr) {
				transform->RemoveChild(obj->GetTransform());
			}
			(*itr)->parent = nullptr;
			itr = children.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void BaseObject::DeleteChild(BaseObject* child) {
	for (auto itr = children.begin(); itr != children.end();) {
		if (*itr == child) {
			BaseObject* obj = *itr;
			delete obj;
			itr = children.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void BaseObject::DeleteAllChildren() {
	for (auto itr = children.begin(); itr != children.end();) {
		BaseObject* obj = *itr;
		itr = children.erase(itr);
		delete obj;
		
	}
}

void BaseObject::RemoveParent() {
	if (parent) {
		parent->RemoveChild(this);
		parent = nullptr;
	}
}