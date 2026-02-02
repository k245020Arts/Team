#pragma once
#include "../Component.h"
#include "UIBase.h"

class MeshRenderer2D;

class EnemyDamageUI :  public UIBase
{
public:
	EnemyDamageUI();
	~EnemyDamageUI();

	void Update()override;
	void Draw()override;

	void SetInformation(VECTOR3 _offset, int _damageNum, VECTOR3 _movePos, float _timer, int _image, Transform* _world,VECTOR2I _graphsize);

private:

	int numImage;
	Transform* worldTransform;
	VECTOR3 worldOffset;
	int damageNum;
	VECTOR3 movePos;
	VECTOR3 moveMaxPos;
	float timer;
	float timerMax;
	MeshRenderer2D* mesh2D;
	bool feedOut;
	bool feedIn;

	float scale;
	int alpha;
	float digitCount;
	float waitTimer;
	float feedInOutTime;
	float feedInOutTimeMax;
	VECTOR2I graphSize;
};