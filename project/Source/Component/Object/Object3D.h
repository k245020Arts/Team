#pragma once
#include "BaseObject.h"

class Object3D : public BaseObject
{
public:
	Object3D();
	virtual ~Object3D();
	void Update() override;
	void Draw() override;
	//Transformコンポーネントを生成させたりさせる。
	void Init(const VECTOR3 _pos, const VECTOR3 _rotate, const VECTOR3 _scale, std::string name);
	void Init(Transform _transform,std::string name);

private:

};