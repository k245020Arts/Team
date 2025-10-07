#pragma once
#include "../Library/GameObject.h"

class ComponentManager;
class Color;
class TransitorManager;
class Transform;
//Objectの基底クラス
class BaseObject : public GameObject
{
public:
	BaseObject();
	~BaseObject();
	virtual void Update()override;
	virtual void Draw()override;

	ComponentManager* Component() const { return componentManager; }

	//オブジェクトに持たせたりする再生速度の重み
	//プレイヤーとかエネミーとかで再生速度を分けたいときに使う
	void SetObjectTimeRate(float _rate) { objectTimeRate = _rate; }
	//実際に使うときはこっちで使う
	float GetObjectTimeRate() { return objectTimeRate * Time::DeltaTimeRate(); }
	int GetId() { return id; }

	Transform* GetTransform() const { return transform; }

	void Init(std::string name);

	/// <summary>
	/// 子オブジェクトを追加する
	/// </summary>
	/// <param name="child"></param>
	void AddChild(BaseObject* child);
	void AddChild(BaseObject* child, bool _transformParent);
	/// <summary>
	/// 子のオブジェクトを取り外す
	/// </summary>
	/// <param name="child"></param>
	void RemoveChild(BaseObject* child);

	// 子のオブジェクトを削除する
	void DeleteChild(BaseObject* child);

	// 子のオブジェクトをすべて削除する
	void DeleteAllChildren();

	// 親から外れる
	void RemoveParent();

	/// <summary>
	/// 親が子供のリストを取り出したい場合はこの関数を使う
	/// </summary>
	/// <returns></returns>
	std::list<BaseObject*> GetChildren() { return children; }
	/// <summary>
	/// 子が親の情報をとりたい場合はこの関数を使うととれる
	/// </summary>
	/// <returns></returns>
	BaseObject* GetParent() { return parent; }

protected:
	ComponentManager* componentManager;
	float objectTimeRate;
	TransitorManager* transitor;
	int id;
	static int idPlus;

	Transform* transform;

	BaseObject* parent;
	std::list<BaseObject*> children;
	bool transformParent;
};
