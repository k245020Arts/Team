#include "EffectManager.h"
#include "Screen.h"
#include "LoadManager.h"
#include "EffectBase.h"
#include "transform.h"
#include "ComponentManager.h"
#include "Object3D.h"
#include "LoadManager.h"

namespace {
	const float EFFECT_EDGE = -400.0f;
	const float EFFECT_EDGE_PLUS = 400.0f;
}


EffectManager::EffectManager()
{
	Load::LoadEffect("enemyChrage2", ID::ENEMY_FLASH, 50.0f);
	Load::LoadEffect("hitEffect", ID::HIT_EFFECT, 50.0f);
	Load::LoadEffect("specialHit", ID::SPECIAL_HIT_EFFECT, 50.0f);
	Load::LoadEffect("justAvoidEffect", ID::JUST_AVOID_EFFECT, 50.0f);
	Load::LoadEffect("playerFlash", ID::PLAYER_FLASH, 50.0f);
	Load::LoadEffect("aura", ID::PLAYER_AURA, 50.0f);
	Load::LoadEffect("playerHit", ID::PLAYER_HIT, 50.0f);
	Load::LoadEffect("playerAttackEffect", ID::PLAYER_ATTACK, 50.0f);
	Load::LoadEffect("hitSlash", ID::PLAYER_SLASH_ATTACK, 50.0f);
	Load::LoadEffect("enemyAttackCicle", ID::ENEMY_ATTACK_CICLE, 50.0f);
	Load::LoadEffect("AlphaCutoff", ID::EFFECT_WALL, 50.0f);
	//Effekseer_SetGraphicsDeviceDXLib();
	SetDrawOrder(-10000);
}

EffectManager::~EffectManager()
{
	//AllStopEffekseer();
	effect.clear();
}

void EffectManager::DeleteAllEffect()
{
	for (auto e = effect.begin(); e != effect.end(); e++) {
		delete* e;
	}
	effect.clear();
}

void EffectManager::Update()
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();

		if (!base->IsActive()) {
			delete* e;
			(*e) = nullptr;
			e = effect.erase(e);
		}
		else {
			e++;
		}
	}

	UpdateEffekseer2D();
	UpdateEffekseer3D();
}

void EffectManager::Draw()
{
	Effekseer_Sync3DSetting();
	DrawEffekseer3D();
	DrawEffekseer2D();
}

int EffectManager::CreateEffekseer(Transform _transform, BaseObject* _parent, ID::EFFECT_ID _id, float _time, bool effect3D)
{
	Object3D* obj = new Object3D();
	obj->Init(_transform.position, _transform.rotation, _transform.scale, std::to_string(_id));
	EffectBase* base =  obj->Component()->AddComponent<EffectBase>();
	base->EffectInit(_transform,_parent,_id,_time,effect3D);
	effect.emplace_back(obj);
	
	return 0;
}

void EffectManager::StopEffekseer(ID::EFFECT_ID _id)
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();
		if (base->GetEffectID() == _id) {
			base->EffectStop();
			delete* e;
			(*e) = nullptr;
			e = effect.erase(e);
			return;
		}
		else {
			e++;
		}
	}
}

void EffectManager::SetSpeedEffekseer(ID::EFFECT_ID _id,float _speed)
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();
		if (base->GetEffectID() == _id) {
			base->EffectSpeedSet(_speed);
			return;
		}
		else {
			e++;
		}
	}
}

void EffectManager::AllStopEffekseer()
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();	
		base->EffectStop(); 
		delete* e;
		(*e) = nullptr;
		e = effect.erase(e);
	}
}

bool EffectManager::IsPlayIng(ID::EFFECT_ID _id)
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();
		if (base->GetEffectID() == _id) {
			return true;
		}
		else {
			e++;
		}
	}
	return false;
}

void EffectManager::SetColor(ID::EFFECT_ID _id, Color::Rgb _rgb)
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();
		if (base->GetEffectID() == _id) {
			base->SetColor(_rgb);
			e++;
		}
		else {
			e++;
		}
	}
}
