#pragma once
#include "ColliderBase.h"
#include "../Transform/Transform.h"

class CapsuleCollider : public ColliderBase
{
public:
	CapsuleCollider();
	~CapsuleCollider();

	void Update() override;
	void Draw() override;

	void Start()override;

	/// <summary>
	/// “–‚½‚è”»’è‚Ìİ’è
	/// </summary>
	/// <param name="_info">“–‚½‚è”»’è‚Ìî•ñ</param>
	/// <param name="_transform">ü‚Ìæ’[</param>
	/// <param name="_transform2">ü‚Ì––’[</param>
	void CapsuleSet(const CollsionInfo& _info, const Transform& _backTransform, const Transform& _frontTransform,float _sphere);
	void CapsuleSet(const CollsionInfo& _info, const Transform& _backTransform, const Transform& _frontTransform, float _sphere, std::string _tag);
	Transform* CapselBackPosTransform() { return capsuleFrontTransform; }const
	float GetRadius() { return radius; }const

private:
	Transform* capsuleFrontTransform;
	float radius;
};