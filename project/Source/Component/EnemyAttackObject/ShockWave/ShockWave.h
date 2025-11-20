#pragma once
#include "../EnemyAttackObject.h"
#include "../../../Common/ID/EffectID.h"
#include "../../Collider/CollsionInformation.h"

class EffectManager;
class Transform;
class DountCollider;

class ShockWave : public EnemyAttackObject
{
public:
	ShockWave();
	~ShockWave();

	void Update()override;
	void Draw()override;

	void CreateWave(CollsionInformation::Tag _tag, Transform _transform, float _startOutRange, float _speed);

private:

	EffectManager* effect;
	DountCollider* collider;
	float waveSpeed;
};
