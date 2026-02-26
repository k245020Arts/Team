#pragma once
#include "../../../../Library/GameObject.h"

class BossRockBase;
class Boss;
class BaseObject;

class BossRockManager : public GameObject
{
public:
	BossRockManager();
	BossRockManager(Boss* _boss);
	~BossRockManager();

	void Update()override;
	void Draw() override;

	void CreateRock(int _index, int _total, float _rotateAngle);
	void CreateLastRock();

	void PushList(BossRockBase* _obj);
	void RemoveList(BossRockBase* _obj);

	size_t GetSize() { return rocks.size(); }

	bool IsFreePos(const VECTOR3& _pos, float _minDist);

	void ShakeCamera();

	void ThrowStart();
	void CreateThrow(VECTOR3& _addPos);

private:
	std::list<BossRockBase*> rocks;
	Boss* boss;

	void SetRockComponent(BaseObject* _base,const VECTOR3& _gravity, const VECTOR3& _fir);
};
