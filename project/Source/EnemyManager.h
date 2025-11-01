#pragma once
#include "../Library/GameObject.h"
#include <list>

class Object3D;
class BaseObject;
class Camera;
class CharaBase;

class EnemyManager : public GameObject
{
public:
	EnemyManager();
	~EnemyManager();
	void Update() override;
	void Draw() override;

	std::list<Object3D*> GetEnemy();
	std::list<Object3D*>::iterator GetItr();

	void CreateEnemy();
	void PlayerObjPointer();

	void CreateBoss();

	bool PlayerDistance(Camera* camera);
	bool TargetCancel(Camera* camera);
	void JustAvoidTargetChange(Object3D* _obj);

private:
	
	std::list<Object3D*> enemy;
	Object3D* player;
	std::list<CharaBase*> chara;
	BaseObject* cameraTargetObj;
};