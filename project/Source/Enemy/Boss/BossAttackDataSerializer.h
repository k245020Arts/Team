#pragma once
#include <string>
#include <memory>
#include "BossState/Attack/BossAttackBase.h"
#include "../../Component/Animator/Animator.h"

class AttackSorting;
struct ActionParam;

class BossAttackDataSerializer
{
public:
	BossAttackDataSerializer();
	BossAttackDataSerializer(std::shared_ptr<AttackSorting> _sort,std::string _bossName);
	~BossAttackDataSerializer();

	void SetAnim(Animator* anim);

	void Update();

private:
	bool windowMode;
	std::string BossName;
	std::shared_ptr<AttackSorting> sorting;
	std::unordered_map<std::string, BossAttackBase::BossAttackParam> attackParam;
	std::vector<ActionParam> actions;
	std::vector<std::string> attackKeys;
	char newAttackID[64];
	char newAnimFile[128];

	void ActionsSave();
	void AttackSave(std::string _attackID);
	void DrawAttackParamEditor(std::string _selectID);
	void DrawTransform(const char* label, Transform& t);
	void CopyParam(std::string _selectID);
	void DrawThrowObjectEditor(std::vector<BossAttackBase::ThrowObjectAttackData>& list);
	void DrawRayColliderInfo(const char* label, BossAttackBase::RayColliderInfo& r);
	void DrawDountColliderInfo(const char* label, BossAttackBase::DountColliderInfo& d);
	Animator* bossAnim;
	Animator::AnimFileInfo currentSelectAnimInfos;

	std::vector<std::string> animFileName;
};