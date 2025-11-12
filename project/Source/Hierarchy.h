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
	/// <summary>
	/// デバック画面リストに追加
	/// </summary>
	/// <param name="_name"></param>
	/// <param name="_obj"></param>
	void Start(std::string _name, BaseObject* _obj);

	/// <summary>
	/// 実際に描画をしているところunityでいうヒエラルキーの描画
	/// </summary>
	/// <param name="_obj"></param>
	void DrawHierarchy(BaseObject* _obj);
	/// <summary>
	/// 実際にパラメータとかを出しているところ、unityでいうところのインスペクターの描画
	/// </summary>
	void InspectorDraw();

	void TransformDraw(BaseObject* _obj);
	void RemoveHierachy(BaseObject* _obj);
	/// <summary>
	/// すべてのデータを削除
	/// </summary>
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