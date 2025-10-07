#pragma once

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
		WALL,

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
