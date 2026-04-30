#pragma once
#include "../../TrashEnemy/EnemyState/EnemyStateBase.h"

class AttackSorting;
class BossStatus;

class BossRun : public EnemyStateBase
{
public:
	BossRun();
	~BossRun();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	BossStatus* bs;
	VECTOR3 velocity;
	VECTOR3 rotation;

	VECTOR3 targetPos;
	//Šâ‚ð‚æ‚¯‚é
	void HitLine();
	//‹ß‚­‚ÌŠâ‚Ìƒ|ƒWƒVƒ‡ƒ“‚ð“ü‚ê‚é
	std::list<VECTOR3> RayCastHits();
	float speed;

	bool islooping;
	float counter;

};