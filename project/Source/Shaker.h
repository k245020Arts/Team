#pragma once
#include "../Library/myDxLib.h"
#include "../Library/Time.h"
#include "Component.h"
#include "transform.h"

class Shaker : public Component
{
public:
	enum ShakePattern
	{
		NONE = -1,
		VERTICAL_SHAKE,
		HORIZONAL_SHAKE,
		HEIGHT_SHAKE,
		MIX_SHAKE,
	};
	Shaker();
	~Shaker();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// シェイクを始めたいときに呼ぶとshakeが出来る。
	/// </summary>
	/// <param name="_power">強さ</param>
	/// <param name="_pattern">縦揺れか横揺れかランダムか</param>
	/// <param name="rok">trueにするとヒットストップになってアニメーションとかも全部止まるようになる。falseにすると普通に動く</param>
	/// <param name="_second">秒数</param>
	/// <param name="_baseTransform">自身のtransform(obj->GetTransform())を使うのではなく、別のtransformを使いたいときはこれを書く。</param>
	void ShakeStart(VECTOR3 _power,ShakePattern _pattern,float rok,float _second);
	void ShakeStart(VECTOR3 _power,ShakePattern _pattern,float rok,float _second,Transform* _baseTransform);
	

	void SetShakePower(VECTOR3 _power);
	void AddShakePower(VECTOR3 _power);
	
	void ShakeFinish();
	bool IsShakeing() { return (time > 0 || time < -1.0f); }

	VECTOR3 GetShakePos() const ;
	VECTOR3 GetCurrentPos() { return currentTransform.position; }
	VECTOR3 GetShakePower() { return currentShakePower; }

	VECTOR3 GetShakeOffset() const;

	void ImguiDraw()override;

private:

	Transform currentTransform;
	Transform* transform;

	VECTOR3 power;
	VECTOR3 powerSub;
	float time;
	ShakePattern pattern;
	bool stop;

	void ShakePower();
	VECTOR3 GetShake(VECTOR3 _shake);

	VECTOR3 shakePos;

	VECTOR3 shakeSubPosition;
	VECTOR3 currentShakePower;
};
