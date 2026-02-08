#pragma once
#include <vector>
#include "../../../Library/myDxLib.h"

class Transform;
class ColliderBase;

namespace CollsionInformation {
	//当たり判定の形
	enum Shape
	{
		SPHERE = 0,
		MODEL,
		RAY,
		AABB,
		DONUT,
		SHAPE_MAX,
	};
	//当たり判定の種類
	enum Tag
	{
		ENEMY,
		PLAYER,
		E_ATTACK,
		P_ATTACK,
		CAMERA,
		WALL,
		BOSS,
		B_ATTACK,
		FLOOR,
		E_FLOOR,
		P_FLOOR,
		B_FLOOR,
		C_FLOOR,
		SHADOW,
		B_E_ATTACK,
		P_SPECIAL_ATTACK,
		BOSS_PUSH,

		TAG_MAX,
	};

	constexpr int COLLSION_KIND_MAX = SHAPE_MAX * SHAPE_MAX;
	constexpr int COLLSION_TAG_MAX = TAG_MAX * TAG_MAX;

	/// <summary>
	/// 当たり判定をどのオブジェクト同士で行うかを設定する関数。初めに一回呼び出している。
	/// </summary>
	void InitSetPair();
	//そのペアが当たり判定を行ってよいかを判定
	bool IsCollPair(Tag tag1, Tag tag2);

}

struct CollsionInfo
{
	CollsionInfo()
	{
		shape = CollsionInformation::SHAPE_MAX;
		tag = CollsionInformation::TAG_MAX;
		size = 0.0f;
		parentTransfrom = nullptr;
		oneColl = false;
	}
	CollsionInformation::Shape shape;
	CollsionInformation::Tag tag;
	float size;
	Transform* parentTransfrom;
	bool oneColl;
};

class Transform;
class Physics;

struct PushInfo {
	VECTOR3 normal;      // 法線ベクトル
	float penetration; // めり込み量
	CollsionInformation::Shape shape;
	VECTOR3 targetPos;

	PushInfo() {

		normal = VZero;
		penetration = 0.0f;
		shape = CollsionInformation::SHAPE_MAX;
		targetPos = VZero;
	}

	PushInfo(const VECTOR3& n, float p, CollsionInformation::Shape _shape,VECTOR3 _targetPos)
		: normal(n), penetration(p) ,shape(_shape) , targetPos(_targetPos) {
	}
};

class Pushback {
public:
	Pushback();
	~Pushback();
	/// <summary>
	/// 当たり判定情報を削除
	/// </summary>
	void Clear();

	/// <summary>
	/// 押し返しデータのセット
	/// </summary>
	/// <param name="normal">法線</param>
	/// <param name="penetration">押し返しの量</param>
	/// <param name="_shape">形</param>
	/// <param name="_targetPos">押し返しに使う相手の当たり判定</param>
	void AddPush(const VECTOR3& _normal, float _penetration, CollsionInformation::Shape _shape, const VECTOR3& _targetPos);

	// 押し返しベクトルを計算して返す
	VECTOR3 ResultPushback(float  _maxLength = 5.0f);

	/// <summary>
	/// 押し返しを適用
	/// </summary>
	/// <param name="transform">当たり判定に使用するTransform</param>
	/// <param name="physics">フィジックスのポインタ</param>
	/// <param name="affectVelocity">velocityを適用するか</param>
	/// <param name="maxLength">最大の長さ</param>
	void Apply(Transform* _transform, Physics* _physics, bool _affectVelocity = true, float _maxLength = 5.0f);
	bool IsGrounded(float _minYNormal = 0.6f);
	std::vector<PushInfo>& GetPushInfo() { return pushes; }
private:
	std::vector<PushInfo> pushes;
};