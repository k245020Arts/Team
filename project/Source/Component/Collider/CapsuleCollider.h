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
	void CapsuleSet(const CollsionInfo& _info, const Transform& _endTransform, const Transform& _startTransform,float _sphere,bool _mustMatrix,MATRIX* _matrix);
	void CapsuleSet(const CollsionInfo& _info, const Transform& _endTransform, const Transform& _startTransform, float _sphere, bool _mustMatrix, MATRIX* _matrix, std::string _tag);
	Transform* CapselEndTransform() { return capselEndTransform; }
	float GetRadius() { return radius; }
	bool GetMultMatrix() { return matrixMult; }
	void SetCapsuleEndPos(const VECTOR3 _position) { baseEndTransform.position = _position; }
	void AddCapsuleEndPos(const VECTOR3 _add) { baseEndTransform.position += _add; }

private:
	Transform* capselEndTransform;
	float radius;
	bool matrixMult;
	Transform baseStartTransform;
	Transform baseEndTransform;
	MATRIX* matrix;
};