#pragma once
#include "Component.h"

//2Dの画像を描画するときに使うクラス。

class Anim2D;

class MeshRenderer2D : public Component
{
public:
	//drawに使う関数の指定を行う列挙型
	enum GraphMode
	{
		DRAW_GRAPH,
		DRAW_RECT_GRAPH,
		DRAW_RECT_ROTA_GRAPH,
		DRAW_NUM,
		DRAW_RECT_ROTA_GRAPH_FAST_3F,
	};

	MeshRenderer2D();
	~MeshRenderer2D();

	void Update()override;
	void Draw()override;

	//初期化処理
	void TextureHandle(int _image,GraphMode _mode);
	void SetTransform(const VECTOR3 _pos, const VECTOR3 _rotate, const VECTOR3 _scale);
	void ImguiDraw()override;

	void SetStartPos(VECTOR2I _pos);

	void AnimStart(float _speed,int _num);

	VECTOR2I GetImageSize() { return imageSize; }

	void SetNum(int _num) { num = _num; }
	
	void SetDraw(bool _draw) { draw = _draw; }

private:
	int hImage;
	GraphMode mode;
	VECTOR2I imageSize;
	Transform* transform2D;

	Anim2D* anim2D;
	VECTOR2I startPos;
	int num;

	bool draw;

	void DrawNum();
};