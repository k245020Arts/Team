#pragma once
#include "BossRockBase.h"
#include "../../Transform/Transform.h"

class BossRock : public BossRockBase
{
public:
	BossRock();
	~BossRock();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// 当たり判定の開始
	/// </summary>
	/// <param name="_tag">当たり判定のタグ</param>
	/// <param name="_transform">親にするトランスフォーム</param>
	void StartCollAdd(CollsionInformation::Tag _tag, const Transform& _transform);
	/// <summary>
	/// 当たり判定の取り除き
	/// </summary>
	/// <param name="_name">当たり判定のタグ///</param>
	void RemoveCollider(std::string _name);

	void SetPreInfo(const VECTOR3& _pos);

	void Ground()override;


private:

	CollsionInformation::Tag collTag;

	float time;
	bool preDraw;
	Transform preTransform;
	int preModel;
	

};
