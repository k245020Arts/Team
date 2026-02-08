#pragma once
#include "../../../Library/GameObject.h"
#include <EffekseerForDXLib.h>
#include "../../Component/Component.h"
#include "../../Component/Transform/Transform.h"
#include "../ID/EffectID.h"
#include "../../Component/Color/Color.h"

class BaseObject;

class EffectBase : public Component
{
public:
	EffectBase();
	~EffectBase();
	void Update()override;
	void Draw()override;

	/// <summary>
	/// エフェクトの準備をするクラス
	/// EffectManagerで自動で呼ばれるので手動で呼ぶ必要はない
	/// </summary>
	/// <param name="_transform"></param>
	/// <param name="_parent"></param>
	/// <param name="_id"></param>
	/// <param name="_time"></param>
	/// <param name="effect3D"></param>
	void EffectInit(Transform _transform, BaseObject* _parent, Effect_ID::EFFECT_ID _id, float _time, bool effect3D = true);

	/// <summary>
	/// エフェクトを再生させる2Dと3Dで関数を分けている
	/// </summary>
	void EffectPlay2D();
	void EffectPlay3D();
	/// <summary>
	/// エフェクトをストップさせる2Dと3Dで関数を分けている
	/// </summary>
	void EffectStop2D();
	void EffectStop3D();

	void ChangeSpeed(float _speed) { speed = _speed; }

	/// <summary>
	/// 現在使用してるエフェクトのIDをわたす
	/// </summary>
	/// <returns>エフェクトのID</returns>
	Effect_ID::EFFECT_ID GetEffectID() { return id; }
	/// <summary>
	/// エフェクトの再生を止める
	/// </summary>
	void EffectStop();
	
	/// <summary>
	/// エフェクトのスピードを調整
	/// </summary>
	/// <param name="_speed">基準を1としたときのスピード</param>
	void EffectSpeedSet(float _speed);
	/// <summary>
	/// エフェクトの色を変える2Dver
	/// </summary>
	void SetColor2D(Color::Rgb _rgb);
	/// <summary>
	///  エフェクトの色を変える3Dver
	/// </summary>
	/// <param name="_rgb">RGBの構造体</param>
	void SetColor3D(Color::Rgb _rgb);
	void SetColor(Color::Rgb _rgb);

	void ParentTransformRemove();

private:

	Transform* transform;
	BaseObject* parent;
	Effect_ID::EFFECT_ID id;

	typedef void (EffectBase::* EffectKind)();
	EffectKind effectPlay;
	typedef void (EffectBase::* Stop)();
	Stop effectStop;

	typedef void (EffectBase::* EfColor)(Color::Rgb _rgb);
	EfColor effectColor;
	
	int hPlayHandle;
	float time;
	float speed;
	float feedInTime;
	const float FEEDIN_TIME = 0.1f;
};
