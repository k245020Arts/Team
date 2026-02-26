#pragma once
#include "BossRockBase.h"

class BossThrowRock : public BossRockBase
{
public:
	BossThrowRock();
	~BossThrowRock();

	void Update()override;
	void Draw()override;

	void Ground()override;

	void CreateThrowRock(VECTOR3& _addPos);
	void ThrowRockStart(BaseObject* _player);

private:
	bool throwRock;
	VECTOR3 dir;
	int throwRockBaseModel;
	int boneNum;
	bool throwReady;
	VECTOR3 addPos;
};
