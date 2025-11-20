#pragma once
#include "../../../Library/GameObject.h"
#include <EffekseerForDXLib.h>
#include "../../Component/Color/Color.h"
#include "../ID/EffectID.h"

class Transform;
class BaseObject;

class EffectManager : public GameObject
{
public:
	EffectManager();
	~EffectManager();

	void DeleteAllEffect();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// エフェクトの生成を行う関数
	/// </summary>
	/// <param name="_transform">発生させるTransformを設定</param>
	/// <param name="_parent">親をもつなら親のポインタを持つ</param>
	/// <param name="_id">エフェクトのID</param>
	/// <param name="_time">エフェクトの発生させる時間</param>
	/// <param name="effect3D">エフェクト3Dにするか2Dにするかデフォルトは3D</param>
	/// <returns></returns>
	BaseObject* CreateEffekseer(Transform _transform, BaseObject* _parent, Effect_ID::EFFECT_ID _id,float _time,bool effect3D = true);
	/// <summary>
	/// エフェクトの再生を止める
	/// </summary>
	/// <param name="_id"></param>
	void StopEffekseer(Effect_ID::EFFECT_ID _id);
	/// <summary>
	/// エフェクトのスピード調整
	/// </summary>
	/// <param name="_id"></param>
	/// <param name="_speed"></param>
	void SetSpeedEffekseer(Effect_ID::EFFECT_ID _id,float _speed);
	void AllStopEffekseer();
	/// <summary>
	/// エフェクトが再生されているか
	/// </summary>
	/// <param name="_id"></param>
	/// <returns></returns>
	bool IsPlayIng(Effect_ID::EFFECT_ID _id);
	/// <summary>
	/// エフェクトの色を変えたいときに使う。
	/// 主にalphaで使うことが多いかも？
	/// </summary>
	/// <param name="_id"></param>
	/// <param name="_rgb"></param>
	void SetColor(Effect_ID::EFFECT_ID _id, Color::Rgb _rgb);
	
private:
	std::list<BaseObject*> effect;
};