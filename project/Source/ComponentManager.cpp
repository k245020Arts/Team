#include "ComponentManager.h"

ComponentManager::~ComponentManager()
{
	for (auto c = component.begin(); c != component.end();c++) {
		delete (*c);
		*c = nullptr;
	}
	component.clear();
}

void ComponentManager::Update()
{
	//とうろくしたコンポーネントを回している。
	for (auto c = component.begin(); c != component.end();) {
		if ((*c)->IsActive()) {
			(*c)->Update();
			c++;
		}
		else {
			delete (*c);
			*c = nullptr;
		}
	}
}

void ComponentManager::Draw()
{
	//とうろくしたコンポーネントを回している。
	for (auto c = component.begin(); c != component.end();) {
		(*c)->Draw();
		c++;
	}
}