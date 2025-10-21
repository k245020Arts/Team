#include "CollsionInformation.h"
#include <array>
#include "function.h"
#include "transform.h"
#include "Physics.h"
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
    SetPair(PLAYER, E_ATTACK);
    SetPair(PLAYER, WALL);
    //SetPair(CAMERA, WALL);
    SetPair(ENEMY, P_ATTACK);
    SetPair(ENEMY, WALL);
    SetPair(PLAYER, B_ATTACK);
    SetPair(BOSS, P_ATTACK);
    SetPair(BOSS, WALL);
    SetPair(FLOOR, P_FLOOR);
    SetPair(FLOOR, E_FLOOR);
    SetPair(FLOOR, C_FLOOR);
    SetPair(FLOOR, B_FLOOR);
}

bool CollsionInformation::IsCollPair(Tag tag1, Tag tag2)
{
    return  pairTag[Function::EnumTag(tag1, tag2, CollsionInformation::TAG_MAX)];
}

PushbackResolver::PushbackResolver()
{
}

PushbackResolver::~PushbackResolver()
{
    Clear();
}

void PushbackResolver::Clear() {
    pushes.clear();
}

void PushbackResolver::AddPush(const VECTOR3& normal, float penetration, CollsionInformation::Shape _shape)
{
    if (penetration > 0.0f) {
        PushInfo i(normal, penetration, _shape);

        pushes.emplace_back(i);
    }
}

VECTOR3 PushbackResolver::ResolvePushback(float maxLength){
    if (pushes.empty()) return VECTOR3(0, 0, 0);

    VECTOR3 totalPush = VECTOR3(0, 0, 0);

    for (const auto& p : pushes) {
        VECTOR3 newPush = p.normal * p.penetration;

		totalPush += newPush;
		VECTOR3 pushVecNorm = totalPush.Normalize();
		float pushIn = VDot(pushVecNorm, newPush);
		if (pushIn < totalPush.Size()) {
			totalPush += newPush - pushIn * pushVecNorm;
		}
		else {
			totalPush = newPush;
		}
    }

    // 正規化 & 最大長クランプ（安全性）
    if (totalPush.Size() > maxLength) {
        totalPush = totalPush.Normalize() * maxLength;
    }

    float magnitude = totalPush.Size();
    float smoothMag = 0.5f * 0.5f + magnitude * (1.0f - 0.5f);
    VECTOR3 result = totalPush.Normalize() * smoothMag;

    return totalPush;

    //if (pushes.empty()) return VECTOR3(0, 0, 0);

    //VECTOR3 totalPush = VECTOR3(0, 0, 0);

    //for (const auto& p : pushes) {
    //    totalPush += p.normal * p.penetration;
    //}

    //// 正規化 & 最大長クランプ（安全性）
    //if (totalPush.Size() > maxLength) {
    //    totalPush = totalPush.Normalize() * maxLength;
    //}

    //return totalPush;
}

void PushbackResolver::Apply(Transform* transform, Physics* physics, bool affectVelocity, float maxLength) {
    VECTOR3 push = ResolvePushback(maxLength);
    if (push == VECTOR3(0, 0, 0)) {
        //lastPushMagnitude = 0.0f;
        return;
    }

    // 位置補正（押し返し）
    transform->position += push;

    // 押し返しに応じた速度補正（接地してる場合のみ）
    //physics->SetGravity(IsGrounded());
    if (affectVelocity && physics) {
        if (IsGrounded()) {
            // 重力方向に限ってキャンセル
            VECTOR3 gravityOnly = VECTOR3(0, 1, 0);
            VECTOR3 velocity = physics->GetVelocity();
            VECTOR3 normal = push.Normalize();
            float gDot = VDot(gravityOnly, normal);
            float dot = VDot(gravityOnly, velocity);
            if (gDot > 0.5f) {
                
                VECTOR3 cancel = normal * -dot;
                physics->AddVelocity(cancel, false);  // ← Y方向だけにする
            }
           
        }
        else {
            VECTOR3 velocity = physics->GetVelocity();
            VECTOR3 normal = push.Normalize();
            float dot = VDot(normal, velocity);

            if (dot < -1.0f) {
                // めり込んでる方向の速度を殺す
                physics->AddVelocity(normal * -dot, false);
            }
        }
    }

   // lastPushMagnitude = push.Size();
}

bool PushbackResolver::IsGrounded(float minYNormal) const {
    for (const auto& push : pushes) {
        // 法線のY成分が一定以上なら「地面」とみなす
        if (push.normal.y >= minYNormal) {
            return true;
        }
    }
    return false;
}