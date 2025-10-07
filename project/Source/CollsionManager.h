#pragma once
#include "../Library/GameObject.h"
#include <array>
#include "CollsionInformation.h"

class Object3D;
class ColliderBase;
class CollsionEvent;

//“–‚½‚è”»’è‚Ì‘‚Ü‚Æ‚ß‚ğ‚·‚éƒNƒ‰ƒXB

class CollsionManager : public GameObject
{
public:
	CollsionManager();
	~CollsionManager();
	void Update()override;
	void Draw()override;
	void AddPush(ColliderBase* obj);

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