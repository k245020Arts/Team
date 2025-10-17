#include "CollsionInformation.h"
#include <array>
#include "function.h"

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
    SetPair(CAMERA, WALL);
    SetPair(ENEMY, P_ATTACK);
    SetPair(ENEMY, WALL);
    SetPair(PLAYER, B_ATTACK);
    SetPair(BOSS, P_ATTACK);
}

bool CollsionInformation::IsCollPair(Tag tag1, Tag tag2)
{
    return  pairTag[Function::EnumTag(tag1, tag2, CollsionInformation::TAG_MAX)];
}
