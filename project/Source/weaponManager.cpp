#include "weaponManager.h"
#include "Object3D.h"
#include "transform.h"
#include "MeshRenderer.h"
#include "LoadManager.h"
#include "CharaWeapon.h"
#include "ComponentManager.h"

WeaponManager::WeaponManager()
{
	eObj = nullptr;
	pObj = nullptr;
	eWeapon = nullptr;
	pWeapon = nullptr;
}

WeaponManager::~WeaponManager()
{
}

void WeaponManager::Update()
{
}

void WeaponManager::Draw()
{
}

void WeaponManager::CreatePlayerWeapon()
{ 
	pObj =  new Object3D();
	pObj->Init((VECTOR3)MV1GetFramePosition(Load::GetHandle(ID::P_MODEL), 28), VZero, VECTOR3(30, 30, 30), "PLAYER_WEAPON");

	Object3D* player = FindGameObjectWithTag<Object3D>("PLAYER");
	
	pWeapon = pObj->Component()->AddComponent<CharaWeapon>();
	
	//CharaWeapon* weaponL = pW->Component()->AddComponent<CharaWeapon>();

	//pWeapon->ObjectPointer(player,57,ID::P_MODEL, Load::LoadModel(Load::MODEL_PATH + "Sword", ID::IDType::P_WEAPON));
	pWeapon->ObjectPointer(player,28,ID::P_MODEL, Load::LoadModel(Load::MODEL_PATH + "Sword", ID::IDType::P_WEAPON));
	pWeapon->SetImage(Load::GetHandle(ID::SWORD_EFFECT));
	//pWeapon->CreateSwordEffect();
	//weaponL->ObjectPointer(player,9,ID::P_MODEL, Load::LoadModel(Load::MODEL_PATH + "Sword", ID::IDType::P_WEAPON));
}

void WeaponManager::CreateEnemyWeapon()
{
	eObj = new Object3D();
	eObj->Init((VECTOR3)MV1GetFramePosition(Load::GetHandle(ID::E_MODEL), 28), VECTOR3(0.0f,0.0f,90.0f * DegToRad), VECTOR3(1, 1, 1), "ENEMY_WEAPON");

	Object3D* enemy = FindGameObjectWithTag<Object3D>("ENEMY");

	eWeapon = eObj->Component()->AddComponent<CharaWeapon>();

	eWeapon->ObjectPointer(enemy, 10, ID::E_MODEL, -1);
	eWeapon->SetImage(Load::GetHandle(ID::SWORD_EFFECT));
	//wea->CreateSwordEffect();
}

void WeaponManager::CreateTrailPlayer(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, float index, float _time)
{
	pWeapon->CreateSwordEffect(_nPos,_fPos,_r,_g,_b,_a,index,_time);
}

void WeaponManager::CreateTrailEnemy(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, float index, float _time)
{
	eWeapon->CreateSwordEffect(_nPos, _fPos, _r, _g, _b, _a, index, _time);
}
