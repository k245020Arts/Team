#include "EffectBase.h"
#include "Object3D.h"
#include "LoadManager.h"

EffectBase::EffectBase()
{
	speed = 1.0f;
}

EffectBase::~EffectBase()
{
	delete transform;
	(this->*effectStop)();
}

void EffectBase::Update() 
{
	(this->*effectPlay)();
	time -= Time::DeltaTimeRate();
	if (time <= 0.0f) {
		(this->*effectStop)();
		active = false;
	}
}

void EffectBase::Draw() {

}

void EffectBase::EffectInit(Transform _transform, BaseObject* _parent, ID::EFFECT_ID _id, float _time, bool effect3D)
{
	transform = new Transform(_transform);
	if (_parent != nullptr) {
		transform->SetParent(_parent->GetTransform());
	}

	id = _id;
	time = _time;
	
	if (effect3D) {
		effectPlay = &EffectBase::EffectPlay3D;
		effectStop = &EffectBase::EffectStop3D;
		effectColor = &EffectBase::SetColor3D;
		hPlayHandle = PlayEffekseer3DEffect(Load::GetEffectHandle(id));
	}
	else {
		effectPlay = &EffectBase::EffectPlay2D;
		effectStop = &EffectBase::EffectStop2D;
		effectColor = &EffectBase::SetColor2D;
		hPlayHandle = PlayEffekseer2DEffect(Load::GetEffectHandle(id));
	}
	
}

void EffectBase::EffectPlay2D()
{
	Transform worldTrans = transform->WorldTransform();

	SetPosPlayingEffekseer2DEffect(hPlayHandle, worldTrans.position.x, worldTrans.position.y, worldTrans.position.z);
	SetRotationPlayingEffekseer2DEffect(hPlayHandle, worldTrans.rotation.x, worldTrans.rotation.y, worldTrans.rotation.z);
	SetScalePlayingEffekseer2DEffect(hPlayHandle, worldTrans.scale.x, worldTrans.scale.y, worldTrans.scale.z);

	SetSpeedPlayingEffekseer2DEffect(hPlayHandle, speed);
}

void EffectBase::EffectPlay3D()
{
	Transform worldTrans = transform->WorldTransform();

	SetPosPlayingEffekseer3DEffect(hPlayHandle, worldTrans.position.x, worldTrans.position.y, worldTrans.position.z);
	SetRotationPlayingEffekseer3DEffect(hPlayHandle, worldTrans.rotation.x, worldTrans.rotation.y, worldTrans.rotation.z);
	SetScalePlayingEffekseer3DEffect(hPlayHandle, worldTrans.scale.x, worldTrans.scale.y, worldTrans.scale.z);

	SetSpeedPlayingEffekseer3DEffect(hPlayHandle, speed);
}

void EffectBase::EffectStop2D()
{
	StopEffekseer2DEffect(hPlayHandle);
}

void EffectBase::EffectStop3D()
{
	StopEffekseer3DEffect(hPlayHandle);
}

void EffectBase::EffectStop()
{
	(this->*effectStop)();
	active = false;
}

void EffectBase::EffectSpeedSet(float _speed)
{
	speed = _speed;
}

void EffectBase::SetColor2D(Color::Rgb _rgb)
{
	SetColorPlayingEffekseer2DEffect(hPlayHandle,(int) _rgb.r, (int)_rgb.g, (int)_rgb.b, (int)_rgb.a);
}

void EffectBase::SetColor3D(Color::Rgb _rgb)
{
	SetColorPlayingEffekseer3DEffect(hPlayHandle, (int)_rgb.r, (int)_rgb.g, (int)_rgb.b, (int)_rgb.a);
}

void EffectBase::SetColor(Color::Rgb _rgb)
{
	(this->*effectColor)(_rgb);
}
