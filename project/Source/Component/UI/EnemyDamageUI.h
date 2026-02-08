#pragma once
#include "../Component.h"
#include "UIBase.h"

class MeshRenderer2D;

class EnemyDamageUI :  public UIBase
{
public:
	EnemyDamageUI();
	~EnemyDamageUI();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// ダメージUIの表記
	/// </summary>
	/// <param name="_offset">どれだけずらすか</param>
	/// <param name="_damageNum">描画をする文字</param>
	/// <param name="_movePos">移動する座標</param>
	/// <param name="_timer">表記される時間</param>
	/// <param name="_image">画像イメージ</param>
	/// <param name="_world">親のTransform</param>
	/// <param name="_graphsize">画像サイズ</param>
	void SetInformation(const VECTOR3& _offset, int _damageNum, const VECTOR3& _movePos, float _timer, int _image, Transform* _world,const VECTOR2I& _graphsize);

private:

	int numImage;
	Transform* worldTransform;
	VECTOR3 worldOffset;
	int damageNum;
	VECTOR3 movePos;
	VECTOR3 moveMaxPos;
	float timer;
	float timerMax;
	MeshRenderer2D* mesh2D;
	bool feedOut;
	bool feedIn;

	float scale;
	int alpha;
	float digitCount;
	float waitTimer;
	float feedInOutTime;
	float feedInOutTimeMax;
	VECTOR2I graphSize;
};