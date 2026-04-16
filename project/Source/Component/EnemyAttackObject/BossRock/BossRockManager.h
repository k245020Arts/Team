#pragma once
#include "../../../../Library/GameObject.h"
#include "../../../Enemy/Boss/BossState/Attack/BossAttackBase.h"
#include "../../../Common/JsonReader.h"

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

	void CreateThrowObject(const std::vector<BossAttackBase::ThrowObjectAttackData>& _data, int _index, int _total, float _rotateAngle);
	VECTOR3 GetPushCollSize(BossAttackBase::ThrowObjectAttackData _data);

	void CreateRock(int _index, int _total, float _rotateAngle);
	void CreateLastRock();

	void PushList(BossRockBase* _obj);
	void RemoveList(BossRockBase* _obj);

	size_t GetSize() { return rocks.size(); }

	bool IsFreePos(const VECTOR3& _pos, float _minDist);

	void ShakeCamera();

	void ThrowStart();
	void CreateThrow(VECTOR3& _addPos);
	void DropRockStart();

	//指定されたポジションから一番近い岩のポジションを返す
	VECTOR3 GetRockPos(VECTOR3 _pos);
	std::list<VECTOR3> GetAllRockPos();

	struct BossThrowObjectData
	{
		std::string id;
		std::string modelName;
		int modelData;
		Transform modelTransform;
		Transform pushTransform;
	};

private:

	std::map<std::string,BossThrowObjectData> throwObjectsData;

	std::list<BossRockBase*> rocks;
	Boss* boss;

	void SetRockComponent(Object3D* _base,const VECTOR3& _gravity, const VECTOR3& _fir,const BossAttackBase::ThrowObjectAttackData& _data);
};

inline void to_json(JSON& j, const BossRockManager::BossThrowObjectData& data)
{
	j = JSON{
		{"id", data.id},
		{"modelName", data.modelName},
		{"modelData", data.modelData},
		{"modelTransform", data.modelTransform},
		{"pushTransform", data.pushTransform}
	};
}

// from_json
inline void from_json(const JSON& j, BossRockManager::BossThrowObjectData& data)
{
	j.at("id").get_to(data.id);
	j.at("modelName").get_to(data.modelName);
	j.at("modelData").get_to(data.modelData);
	j.at("modelTransform").get_to(data.modelTransform);
	j.at("pushTransform").get_to(data.pushTransform);
}