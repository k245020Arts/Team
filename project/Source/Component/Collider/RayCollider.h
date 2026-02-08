#pragma once
#include "colliderBase.h"

class RayCollider : public ColliderBase
{
public:
	RayCollider();
	~RayCollider();

	void Update() override;
	void Draw() override;

	void Start()override;

	/// <summary>
	/// “–‚½‚è”»’è‚Ìİ’è
	/// </summary>
	/// <param name="_info">“–‚½‚è”»’è‚Ìî•ñ</param>
	/// <param name="_transform">ü‚Ìæ’[</param>
	/// <param name="_transform2">ü‚Ì––’[</param>
	void RaySet(const CollsionInfo& _info, const Transform& _transform,const Transform& _transform2);
	Transform* Get2Transform() { return transform2; }

private:
	Transform* transform2;
};