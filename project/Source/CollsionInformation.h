#pragma once
#include <vector>
#include "../Library/myDxLib.h"

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

	PushInfo(const VECTOR3& n, float p, CollsionInformation::Shape _shape)
		: normal(n), penetration(p) ,shape(_shape) {
	}
};

class Pushback {
private:
	std::vector<PushInfo> pushes;

public:
	Pushback();
	~Pushback();
	void Clear();

	// 押し返しデータを追加
	void AddPush(const VECTOR3& normal, float penetration, CollsionInformation::Shape _shape);

	// 押し返しベクトルを計算して返す
	VECTOR3 ResultPushback(float maxLength = 5.0f);

	// 位置と速度に押し返しを適用
	void Apply(Transform* transform, Physics* physics, bool affectVelocity = true, float maxLength = 5.0f);
	bool IsGrounded(float minYNormal = 0.6f) const;
};