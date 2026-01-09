#include "CollsionInformation.h"
#include <array>
#include "../../Common/Function.h"
#include "../Transform/Transform.h"
#include "../Physics/Physics.h"
#include <algorithm>


namespace {
    std::array<bool, CollsionInformation::COLLSION_TAG_MAX> pairTag;

    void SetPair(CollsionInformation::Tag tag1, CollsionInformation::Tag tag2) {
        pairTag[Function::EnumTag(tag1, tag2, CollsionInformation::TAG_MAX)] = true;
    }

}

void CollsionInformation::InitSetPair()
{
    SetPair(PLAYER, ENEMY);
    SetPair(PLAYER, BOSS);
    SetPair(PLAYER, BOSS_PUSH);
    SetPair(FLOOR, SHADOW);
    SetPair(PLAYER, B_E_ATTACK);
    SetPair(PLAYER, E_ATTACK);
    SetPair(PLAYER, WALL);
    //SetPair(CAMERA, FLOOR);
    SetPair(ENEMY, P_ATTACK);
    SetPair(ENEMY, WALL);
    SetPair(PLAYER, B_ATTACK);
    SetPair(BOSS, P_ATTACK);
    SetPair(BOSS, WALL);
    SetPair(FLOOR, P_FLOOR);
    SetPair(FLOOR, E_FLOOR);
    SetPair(FLOOR, C_FLOOR);
    SetPair(FLOOR, B_FLOOR);
    SetPair(P_SPECIAL_ATTACK, BOSS);
    SetPair(P_SPECIAL_ATTACK, ENEMY);
}

bool CollsionInformation::IsCollPair(Tag tag1, Tag tag2)
{
    return  pairTag[Function::EnumTag(tag1, tag2, CollsionInformation::TAG_MAX)];
}

Pushback::Pushback()
{
    Clear();
}

Pushback::~Pushback()
{
    Clear();
}

void Pushback::Clear() {
    pushes.clear();
}

void Pushback::AddPush(const VECTOR3& normal, float penetration, CollsionInformation::Shape _shape,VECTOR3 _targetPos)
{
    if (penetration > 0.0f) {
        PushInfo i(normal, penetration, _shape,_targetPos);

        pushes.emplace_back(i);
    }
}

VECTOR3 Pushback::ResultPushback(float maxLength) {
    if (pushes.empty()) {
        return VECTOR3(0, 0, 0);
    }
 
    VECTOR3 totalPush = VECTOR3(0, 0, 0);

    for (const auto& p : pushes) {
        VECTOR3 newPush = p.normal * p.penetration;

		totalPush += newPush;
		VECTOR3 pushVecNorm = totalPush.Normalize();
        //押し返す量を取得
		float pushIn = VDot(pushVecNorm, newPush);
        //丸の形状ならyの押し返しを消す
        if (p.shape == CollsionInformation::SPHERE) {
            pushVecNorm = VECTOR3(1,0,1) * pushVecNorm;
            newPush = VECTOR3(newPush.x, 0.0f, newPush.z);
        }
		if (pushIn < totalPush.Size()) {
			totalPush += newPush - pushIn * pushVecNorm;
		}
		else {
			totalPush = newPush;
		}
    }

    //最大の値を越したらそれ以上は押し返さないようにする
    if (totalPush.Size() > maxLength) {
        totalPush = totalPush.Normalize() * maxLength;
    }

    return totalPush;

}

void Pushback::Apply(Transform* transform, Physics* physics, bool affectVelocity, float maxLength) {
    VECTOR3 push = ResultPushback(maxLength);
    if (push.Size() < 0.001f) return; // 微小なら無視

    // 位置補正
    transform->position += push;

    if (affectVelocity && physics) {
        VECTOR3 vel = physics->GetVelocity();
        VECTOR3 pushNorm = push.Normalize();

        // めり込み方向の速度だけ打ち消す
        float dot = VDot(vel, pushNorm);
        if (dot < 0.0f) {
            vel -= pushNorm * dot;
            physics->SetVelocity(vel);
        }
    }
}

bool Pushback::IsGrounded(float minYNormal) const {
    for (const auto& push : pushes) {
        // 法線のY成分が一定以上なら「地面」とみなす
        if (push.normal.y >= minYNormal) {
            return true;
        }
    }
    return false;
}