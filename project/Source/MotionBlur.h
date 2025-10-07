#pragma once
#include "Component.h"
#include <vector>
#include "ID.h"
#include "transform.h"

class Animator;

struct BlurInfo
{

	BlurInfo() {
		baseModel = 1;
		time = 0.0f;
		maxTime = 0.0f;
		alpha = 0.0f;
		animFrame = 0.0f;
		attackID = 0;
		matrix = MGetIdent();
		firstPos = VZero;
		firstMatrix = MGetIdent();
		transform = Transform();
	}
	int baseModel;
	float time;
	float maxTime;
	float alpha;

	float animFrame;
	int attackID;
	MATRIX matrix;

	VECTOR3 firstPos;
	MATRIX firstMatrix;
	Transform transform;
};

//モーションブラーを行うためのクラス.

class MotionBlur : public Component
{
public:
	MotionBlur();
	~MotionBlur();

	void Update()override;
	void Draw()override;

	//初期化するときに行う関数。ベースとなるモデルをnewした直後に設定する。
	void Start(int _model);

	/// <summary>
	/// モーションブラーの開始
	/// </summary>
	/// <param name="_time">継続時間</param>
	/// <param name="_alpha">半透明度</param>
	/// <param name="_id">アニメーションのID</param>
	/// <param name="_modelFrame">アニメーションの移動をなくすためにやる。フレームの番号を書く</param>
	void MosionStart(float _time,float _alpha, ID::IDType _id,int _modelFrame);

private:

	std::vector<BlurInfo> blur;
	int model;
	Animator* anim;
};