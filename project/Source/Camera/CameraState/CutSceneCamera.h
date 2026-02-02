#pragma once

#include <string>
#include "cameraStateBase.h"

class Transform;

class CutSceneCamera : public CameraStateBase
{
public:
    CutSceneCamera();
    ~CutSceneCamera();

    void Update() override;
    void Draw() override;
    void Start() override;
    void Finish() override;

    void StateImguiDraw() override;

    Transform* PlayerEnemyWorldToPos(std::string _name);

private:

    struct CutPositionInformation
    {
        CutPositionInformation() {
            firstPos = VZero;
            FirstRot = MGetIdent();
            lastPos = VZero;
            lastRot = MGetIdent();
            firstSet = false;
        }
        VECTOR3 firstPos;
        MATRIX FirstRot;
        VECTOR3 lastPos;
        MATRIX lastRot;
        bool firstSet;

    };

    struct CutSceneObj
    {
        CutSceneObj() {
            player = CutPositionInformation();
            enemy = CutPositionInformation();
        }
        CutPositionInformation player;
        CutPositionInformation enemy;
    };
    
    float time;
    bool  first;

    CutSceneObj position;

    CutSceneObj target;

    /*VECTOR3 CutSceneInfoSet(CutSceneObj& _info, Transform* _baseTransform,std::string& _name);
    VECTOR3 GetEndTarget(const CutSceneObj& _info,const VECTOR3& _offset);*/

    std::string beforePosName;
    std::string beforeTargetName;

    
    VECTOR3 firstPos;
    VECTOR3 firstTarget;

    //TODOÇ±ÇÍÇç\ë¢ëÃÇ…Ç∑ÇÈÅAç\ë¢ëÃÇ…ÇµÇƒÇ∑Ç¡Ç´ÇËÇ≥ÇπÇƒÇÕÇ¢ÇÈÇ™ãììÆÇ™Ç®Ç©ÇµÇ≠Ç»Ç¡ÇƒÇ¢ÇÈèÛë‘
    VECTOR3 playerFirstPos;
    MATRIX  playerFirstRot;
    VECTOR3 enemyFirstPos;
    MATRIX  enemyFirstRot;

    VECTOR3 playerLastPos;
    MATRIX  playerLastRot;
    VECTOR3 enemyLastPos;
    MATRIX  enemyLastRot;

    bool playerFirstCaptured;
    bool enemyFirstCaptured;

    VECTOR3 playerFirstTargetPos;
    MATRIX  playerFirstTargetRot;
    VECTOR3 enemyFirstTargetPos;
    MATRIX  enemyFirstTargetRot;

    VECTOR3 playerLastTargetPos;
    MATRIX  playerLastTargetRot;
    VECTOR3 enemyLastTargetPos;
    MATRIX  enemyLastTargetRot;

    bool playerFirstTargetCaptured;
    bool enemyFirstTargetCaptured;
};