#include "colliderBase.h"
#include "CollsionManager.h"

ColliderBase::ColliderBase()
{
	collsionEventFunction = nullptr;
	shape = CollsionInformation::SHAPE_MAX;
	collTag = CollsionInformation::TAG_MAX;
	collTransform = nullptr;
	radius = 0.0f;
	oneColl = false;
	finish = false;
}

ColliderBase::~ColliderBase()
{
	if (collTag == CollsionInformation::ENEMY) {
		int a = 0;
	}
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

void ColliderBase::CollsionAdd(const CollsionInfo& _info, const Transform& transform)
{
	CollsionAdd(_info, transform, "");
}

void ColliderBase::CollsionAdd(const CollsionInfo& _info, const Transform& transform, std::string _tag)
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