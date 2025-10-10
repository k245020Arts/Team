#pragma once
#include "Component.h"
#include "colliderBase.h"
#include "ComponentManager.h"
#include "transform.h"

class SphereCollider;

class CharaBase : public Component

{
public:
	CharaBase();
	~CharaBase();

	void Update()override;
	void Draw()override;

	//“–‚½‚è”»’è‚Ì¶¬‚ğ‚µ‚Ä‚¢‚é(UŒ‚‚Ì)
	template<typename T>
	CollsionInfo CollsionInstant(CollsionInformation::Shape _shape, Transform _trans) {
		attackColl = obj->Component()->AddComponent<T>();
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.oneColl = true;
		info.shape = _shape;
		return info;
	}
	//“–‚½‚è”»’è‚ÌŒ`‚Ìî•ñ‚ğ•Û
	void SetShape(CollsionInformation::Shape _shape) { collShape = _shape; }
	//“–‚½‚è”»’è‚Ìíœ
	void DeleteColliderComponent();

	float* GetHp() { return &hp; }

	const float GetMaxHp(){return maxHp;}


	void LastTargetIn() { lastTarget = true; }
	void LastTargetOut() { lastTarget = false; }

	bool GetLastTarget() { return lastTarget; }

protected:
	ColliderBase* attackColl;

	CollsionInformation::Shape collShape;
	std::string collName;

	float hp;
	float maxHp;

	bool lastTarget;
};