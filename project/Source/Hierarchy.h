#pragma once
#include "../Library/GameObject.h"
#include "Component.h"

class Color;
class Object3D;
class InputManager;
class FadeTransitor;
class StateManager;

//デバックウィンドウを出すときに使うクラス。

class Hierachy : public GameObject
{
public:
	Hierachy();
	~Hierachy();
	void Update() override;
	void Draw() override;
	
	void Start(std::string _name, BaseObject* _obj);

	void DrawHierarchy(BaseObject* _obj);
	void InspectorDraw();

	void TransformDraw(BaseObject* _obj);
	void RemoveHierachy(BaseObject* _obj);
	void AllDeleteList();

	

private:
	//Transform* transform;
	std::string name;
	Color* color;
	std::list<BaseObject*> list;

	InputManager* inputManager;
	bool debug;
	FadeTransitor* fade;
	int selectId;
	BaseObject* selectObj;
};