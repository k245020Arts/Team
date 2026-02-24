#pragma once
#include "../../../../Library/GameObject.h"

class BossRock;
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

	void CreateRock(int _createNum);
	void CreateLastRock();

	void PushList(BossRock* _obj);
	void RemoveList(BossRock* _obj);

	size_t GetSize() { return rocks.size(); }

	bool IsFreePos(const VECTOR3& _pos, float _minDist);

	void ShakeCamera();

private:
	std::list<BossRock*> rocks;
	Boss* boss;

	void SetRockComponent(BaseObject* _base,const VECTOR3& _gravity, const VECTOR3& _fir);
};
