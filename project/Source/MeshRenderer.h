#pragma once
#include "Component.h"
#include "color.h"

class Shaker;

//3Dのモデルを描画したいときに行う関数。

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	~MeshRenderer();
	void Update()override;
	void Draw()override;
	
	void ModelHandle(int _hHandle);
	void ImguiDraw()override;

	//プレイヤーのモデルとかが反対になってしまっているのを強制的に直す関数
	void RotationMesh(int _meshNum,float _angle);

	void DrawLocalPosition() { worldTransform = false; }
	void DrawWorldPosition() { worldTransform = true; }
	

private:
	int hModel;
	Color* color;
	int meshNum;
	float angle;

	bool draw;

	Shaker* shaker;
	bool worldTransform;
};