#pragma once
#include "../Library/GameObject.h"
#include <EffekseerForDXLib.h>
#include "Component.h"
#include "transform.h"
#include "EffectID.h"
#include "color.h"

class BaseObject;

class EffectBase : public Component
{
public:
	EffectBase();
	~EffectBase();
	void Update()override;
	void Draw()override;

	void EffectInit(Transform _transform, BaseObject* _parent, Effect_ID::EFFECT_ID _id, float _time, bool effect3D = true);

	void EffectPlay2D();
	void EffectPlay3D();

	void EffectStop2D();
	void EffectStop3D();

	void ChangeSpeed(float _speed) { speed = _speed; }

	Effect_ID::EFFECT_ID GetEffectID() { return id; }
	void EffectStop();
	
	void EffectSpeedSet(float _speed);

	void SetColor2D(Color::Rgb _rgb);
	void SetColor3D(Color::Rgb _rgb);
	void SetColor(Color::Rgb _rgb);

private:

	Transform* transform;
	BaseObject* parent;
	Effect_ID::EFFECT_ID id;

	typedef void (EffectBase::* EffectKind)();
	EffectKind effectPlay;
	typedef void (EffectBase::* Stop)();
	Stop effectStop;

	typedef void (EffectBase::* EfColor)(Color::Rgb _rgb);
	EfColor effectColor;
	
	int hPlayHandle;
	float time;
	float speed;
};
