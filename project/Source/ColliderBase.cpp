#include "colliderBase.h"
#include "CollsionManager.h"

ColliderBase::ColliderBase()
{
}

ColliderBase::~ColliderBase()
{
	delete collTransform;
	CollsionManager* coll = FindGameObject<CollsionManager>();
	coll->RemoveCollList(this);
}

void ColliderBase::Update()
{
}

void ColliderBase::Draw()
{
}

void ColliderBase::CollsionAdd(CollsionInfo _info, Transform transform)
{
	CollsionAdd(_info, transform, "");
}

void ColliderBase::CollsionAdd(CollsionInfo _info,Transform transform,std::string _tag)
{
	shape = _info.shape;
	radius = _info.size;
	oneColl = _info.oneColl;
	collTag = _info.tag;
	finish = false;

	collTransform = new Transform(transform);

	collTransform->SetParent(_info.parentTransfrom);
	
	CollsionManager* coll = FindGameObject<CollsionManager>();
	coll->AddPush(this);
	SetCollTag(_tag);
}