#pragma once
#include "../Library/GameObject.h"
#include <EffekseerForDXLib.h>
#include "color.h"
#include "EffectID.h"

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
	int CreateEffekseer(Transform _transform, BaseObject* _parent, Effect_ID::EFFECT_ID _id,float _time,bool effect3D = true);

	void StopEffekseer(Effect_ID::EFFECT_ID _id);
	void SetSpeedEffekseer(Effect_ID::EFFECT_ID _id,float _speed);
	void AllStopEffekseer();
	bool IsPlayIng(Effect_ID::EFFECT_ID _id);

	void SetColor(Effect_ID::EFFECT_ID _id, Color::Rgb _rgb);
	
private:
	std::list<BaseObject*> effect;
};