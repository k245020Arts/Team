#pragma once
#include "Component.h"
#include "transform.h"

//物理演算関係関数
//基本的にオブジェクトの移動はこのクラスを通して行っている。

class TransitorManager;

class Physics : public Component
{
public:
	Physics();
	~Physics();
	void Update();
	void Start(VECTOR3 _gravityAmout,VECTOR3 _fir);

	//移動をするvelocityに値をいれる
	void AddVelocity(VECTOR3 _addVelocity, bool _deltaTime);
	void SetVelocity(VECTOR3 _velocity) { velocity = _velocity; }

	void AddInterect(VECTOR3 _addInterect, float  _firctionCoeffocoent);
	void SetInterect(VECTOR3 _setInterect, float _firctionCoeffocoent);
	//抵抗を設定する
	void SetFirction(VECTOR3 _setFirction);
	void AddFirction(VECTOR3 _addFirction);
	//重力を設定する
	VECTOR3 GetGravity() { return gravity; }
	void SetGravity(VECTOR3 _setGravity);
	void AddGravity(VECTOR3 _addGraivty);

	VECTOR3 GetFirction() { return firction; }

	VECTOR3 GetVelocity() { return velocity; }

	void ImguiDraw()override;

	Transform* GetLastTransform() { return lastTransform; }
	//ポジションをたす
	void AddPosition(VECTOR3 _pos) { currentTransform->position += _pos; }

	void SetGround(bool _g) { ground = _g; }
	bool GetGround() { return ground; }

private:

	VECTOR3 velocity;//移動する値

	Transform* currentTransform;//今のトランスフォーム

	Transform* lastTransform;//最後のトランスフォーム

	VECTOR3 interectAddPos;

	float frictionCoefficient;
	VECTOR3 gravity; //重力
	VECTOR3 firction; //抵抗、摩擦係数

	bool noGravity;
	bool ground;

	TransitorManager* transitor; //フェード中に移動をするとなぜかすり抜けてしまうことが多々あるのでそれの防止用

};