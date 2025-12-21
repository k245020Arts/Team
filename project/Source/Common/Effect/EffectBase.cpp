#include "EffectBase.h"
#include "../../Component/Object/Object3D.h"
#include "../LoadManager.h"
#include "../Easing.h"

EffectBase::EffectBase()
{
	speed	= 1.0f;
	debugId = 22;
	tag		= Function::GetClassNameC<EffectBase>();
}

EffectBase::~EffectBase()
{
	delete transform;
	(this->*effectStop)();
}

void EffectBase::Update() 
{
	(this->*effectPlay)();
	if (feedInTime <= 0.0f) {
		time		-= Time::DeltaTimeRate();
		if (time <= 0.0f) {
			(this->*effectStop)();
			active	= false;
		}
	}
	else {
		feedInTime		-= Time::DeltaTimeRate();
		if (feedInTime <= 0.0f) {
			feedInTime	= 0.0f;
		}
		float rate		= 1 - (feedInTime / FEEDIN_TIME);
		float amout		= Easing::EaseIn(0.0f, 255.0f, rate);
		Color::Rgb rgb	= Color::Rgb(255.0f, 255.0f, 255.0f, amout);
		(this->*effectColor)(rgb);
	}
	
}

void EffectBase::Draw() {

}

void EffectBase::EffectInit(Transform _transform, BaseObject* _parent, Effect_ID::EFFECT_ID _id, float _time, bool effect3D)
{
	//transform = new Transform(_transform);
	BaseObject* base = GetBaseObject();
	base->GetTransform()->position = _transform.position;
	base->GetTransform()->rotation = _transform.rotation;
	base->GetTransform()->scale = _transform.scale;
	if (_parent != nullptr) {
		parent = _parent;
		parent->AddChild(GetBaseObject(), true);
		//transform->SetParent(_parent->GetTransform());
	}

	id = _id;
	time = _time;
	
	if (effect3D) {
		effectPlay	= &EffectBase::EffectPlay3D;
		effectStop	= &EffectBase::EffectStop3D;
		effectColor = &EffectBase::SetColor3D;
		hPlayHandle = PlayEffekseer3DEffect(Load::GetEffectHandle(id));
	}
	else {
		effectPlay	= &EffectBase::EffectPlay2D;
		effectStop	= &EffectBase::EffectStop2D;
		effectColor = &EffectBase::SetColor2D;
		hPlayHandle = PlayEffekseer2DEffect(Load::GetEffectHandle(id));
	}
	feedInTime		= FEEDIN_TIME;;
}

void EffectBase::EffectPlay2D()
{
	Transform worldTrans = GetBaseObject()->GetTransform()->WorldTransform();
	if (parent != nullptr) {
		worldTrans = parent->GetTransform()->WorldTransform();
	}
	

	SetPosPlayingEffekseer2DEffect(hPlayHandle, worldTrans.position.x, worldTrans.position.y, worldTrans.position.z);
	SetRotationPlayingEffekseer2DEffect(hPlayHandle, worldTrans.rotation.x, worldTrans.rotation.y, worldTrans.rotation.z);
	SetScalePlayingEffekseer2DEffect(hPlayHandle, worldTrans.scale.x, worldTrans.scale.y, worldTrans.scale.z);

	SetSpeedPlayingEffekseer2DEffect(hPlayHandle, speed);
}

void EffectBase::EffectPlay3D()
{
	Transform worldTrans = GetBaseObject()->GetTransform()->WorldTransform();

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

void EffectBase::ParentTransformRemove()
{
	parent->GetTransform()->RemoveChild(obj->GetTransform());
}
