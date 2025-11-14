#pragma once
#include "../Component.h"
#include "CollsionInformation.h"
#include "../Transform/Transform.h"

class ColliderBase : public Component
{
public:
	ColliderBase();
	~ColliderBase();
	void Update();
	void Draw();

	/// <summary>
	/// 当たり判定の追加をするときに呼び出す
	/// タグを付けたいときは下のを使いタグを作る
	/// </summary>
	/// <param name="_info">当たり判定の情報</param>
	/// <param name="transform">当たり判定のTransform</param>
	void CollsionAdd(CollsionInfo _info, Transform transform);
	void CollsionAdd(CollsionInfo _info, Transform transform, std::string _tag);
	

	CollsionInformation::Shape GetShape() { return shape; }
	CollsionInformation::Tag GetCollTag() { return collTag; }

	Transform* GetTransform() { return collTransform; }
	float GetRadius() { return radius * collTransform->scale.x; }
	bool GetOneColl() { return oneColl; }
	bool GetFinish() { return finish; }

	/// <summary>
	/// 当たり判定をもう無視したい場合は個の関数を呼ぶ
	/// </summary>
	void CollsionFinish() { finish = true; }
	/// <summary>
	/// 当たり判定を復活させたいならこの処理を呼ぶ
	/// </summary>
	void CollsionRespown() { finish = false; }
	BaseObject* GetObj() { return obj; }

protected:
	CollsionInformation::Shape shape; //当たり判定の形
	CollsionInformation::Tag collTag; //当たり判定のタグ
	Transform* collTransform;
	float radius;
	bool oneColl;
	bool finish;


};