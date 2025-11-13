#pragma once
#include <list>
#include <typeinfo>
#include "Object3D.h"
#include "Component.h"

//コンポーネントを取りまとめるクラスです。

class ComponentManager
{
public:
	ComponentManager() { obj = nullptr; }
	~ComponentManager();
	void Update();
	void Draw();
	void Init(BaseObject* _ob) { obj = _ob; }

	//コンポーネントを作る。
	template <typename T>
	T* AddComponent() {
		T* com = new T();
		component.emplace_back(com);
		com->Init(obj);
		return com;
	}
	//コンポーネントを削除する。
	template <typename T>
	T* RemoveComponent() {
		for (auto c = component.begin(); c != component.end();) {
			if (typeid(*(*c)) == typeid(T)) {
				delete *c;
				c = component.erase(c);
				return nullptr;
			}
			else {
				c++;
			}
		}
		return nullptr;
	}

	//コンポーネントを削除する。
	template <typename T>
	T* RemoveAllComponent() {
		for (auto c = component.begin(); c != component.end();) {
			if (typeid(*(*c)) == typeid(T)) {
				delete* c;
				c = component.erase(c);
			}
			else {
				c++;
			}
		}
		return nullptr;
	}

	//tagで検索して一致したコンポーネントを削除
	template <typename T>
	T* RemoveComponentWithTag(std::string tag) {
		for (auto c = component.begin(); c != component.end();) {
			if ((*c)->GetTag() == tag) {
				if (typeid(*(*c)) == typeid(T)) {
					delete* c;
					c = component.erase(c);
					return nullptr;
				}
				else {
					c++;
				}
			}
			else {
				c++;
			}
		}
		return nullptr;
	}

	//tagで検索して一致したコンポーネントを削除
	template <typename T>
	T* RemoveComponentWithTagIsCollsion(std::string tag) {
		for (auto c = component.begin(); c != component.end();) {
			if ((*c)->GetCollTag() == tag) {
				if (typeid(*(*c)) == typeid(T)) {
					delete* c;
					c = component.erase(c);
					return nullptr;
				}
				else {
					c++;
				}
			}
			else {
				c++;
			}
		}
		return nullptr;
	}

	template <typename T>
	T* GetComponent() {
		for (Component* c:component) {
			if (typeid(*c) == typeid(T)) {
				return dynamic_cast<T*>(c);
			}
		}
		return nullptr;
	}

	const std::list<Component*>& GetAllComponent() {
		return component;
	}

private:
	std::list<Component*> component;
	BaseObject* obj;
	
};
