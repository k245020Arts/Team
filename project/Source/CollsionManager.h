#pragma once
#include "../Library/GameObject.h"
#include <array>
#include "CollsionInformation.h"

class Object3D;
class ColliderBase;
class CollsionEvent;

//当たり判定の総まとめをするクラス。

class CollsionManager : public GameObject
{
public:
	CollsionManager();
	~CollsionManager();
	void Update()override;
	void Draw()override;
	/// <summary>
	/// 当たり判定リストに追加
	/// </summary>
	/// <param name="obj"></param>
	void AddPush(ColliderBase* obj);
	/// <summary>
	/// 当たり判定リストを削除
	/// </summary>
	/// <param name="obj"></param>
	void RemoveCollList(ColliderBase* obj);

private:
	std::list<ColliderBase*> collList;

	
	typedef bool (CollsionManager::*CollsionKind)(ColliderBase* col1, ColliderBase* col2);
	std::array<CollsionKind, CollsionInformation::COLLSION_KIND_MAX> collsionKind;

	bool CollsionSphereToSphere(ColliderBase* col1, ColliderBase* col2);
	bool CollsionModelToRay(ColliderBase* col1, ColliderBase* col2);
	bool CollsionSphereToModel(ColliderBase* col1, ColliderBase* col2);
	CollsionEvent* event;
};