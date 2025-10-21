#pragma once
#include "Component.h"
#include "Animator.h"
#include "ID.h"

class CharaWeapon : public Component
{
public:
	CharaWeapon();
	~CharaWeapon();

	void Update()override;
	void Draw() override;

	void ObjectPointer(BaseObject* _obj,int _frame, ID::IDType _baseModelName,int _weaponHandle);
	void SetParent(bool _parent);
	void ImguiDraw()override;

	void CreateSwordEffect(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, float index, float _time);

	BaseObject* GetChara() { return chara; }
	int WeaponModel() { return weaponHandle; }
	int CharaModel() { return modelHandle; }

	MATRIX GetWeaponMatrix() { return matrix; }
	void SetImage(int _model);

private:
	BaseObject* chara;
	Animator* anim;
	int frame;
	int modelHandle;
	int weaponHandle;

	bool weapon;
	MATRIX matrix;
	bool parent;
	Transform* transform;
};
