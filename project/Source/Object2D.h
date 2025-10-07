#pragma once
#include "BaseObject.h"

class Object2D : public BaseObject
{
public:
	Object2D();
	virtual ~Object2D() {}
	void Update()override;
	void Draw()  override;

	void Init(const VECTOR2F _pos, const VECTOR2F _rotate, const VECTOR2F _scale, std::string name);
	void Init(Transform _transform, std::string name);

	//void AnimSetting(float _speed, int num);

private:

	//Vector2 imageSize;	// 画像サイズ
	//int hImage;	// 画像ハンドラ

	//float animCounter; //アニメーションのカウンタ
	//int graphNum; //分割の枚数
	//float animSpeed; //アニメーションのスピードの倍率
};