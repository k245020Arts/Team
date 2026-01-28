#pragma once
#include "../Component.h"

class EnemyDamageUI :  public Component
{
public:
	EnemyDamageUI();
	~EnemyDamageUI();

	void Update()override;
	void Draw()override;

private:

	int numImage;
	int damageNum;
	float movePos;
	float timer;
	float timerMax;

};