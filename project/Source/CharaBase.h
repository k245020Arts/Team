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

	//当たり判定の生成をしている(攻撃の)
	//プレイヤーと敵の攻撃の当たり判定の生成を一括で管理したかったのでこういう作りにしています。
	template<typename T>
	CollsionInfo CollsionInstant(CollsionInformation::Shape _shape, Transform _trans) {
		attackColl = obj->Component()->AddComponent<T>();
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.oneColl = true;
		info.shape = _shape;
		return info;
	}
	//当たり判定の形の情報を保持
	void SetShape(CollsionInformation::Shape _shape) { collShape = _shape; }
	//当たり判定の削除
	void DeleteColliderComponent();

	float* GetHp() { return &hp; }

	const float GetMaxHp(){return maxHp;}


	void LastTargetIn() { lastTarget = true; }
	void LastTargetOut() { lastTarget = false; }

	bool GetLastTarget() { return lastTarget; }

	virtual void DeleteCollision();

protected:
	ColliderBase* attackColl;

	CollsionInformation::Shape collShape;
	std::string collName;

	float hp;
	float maxHp;

	bool lastTarget;
};