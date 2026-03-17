#include "PlayerAttackStateBase.h"
#include "../../player.h"
#include "../../../Common/Easing.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../playerStateManager.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Weapon/WeaponManager.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/MotionBlur/MotionBlur.h"
#include "../../../Enemy/EnemyManager.h"
#include "../../../Camera/Camera.h"
#include "../../../Component/Collider/SphereCollider.h"

PlayerAttackStateBase::PlayerAttackStateBase()
{
	easingCount		= 0.0f;
	beforeAngle		= 0.0f;
	angle			= 0.0f;
	nextAttack		= 0.0f;
	distSize		= 0.0f;
	playerAttackData.frontSpeed		= 0.0f;
	playerAttackData.normalAttackNextID = StateID::PLAYER_ATTACK1_S;
	rotation		= 0.0f;
	runTimer		= 0.0f;
	time			= 0.0f;
	beforeAngle		= 0.0f;
	beforeAttack	= false;
	defalutTrail	= true;
	nextAvoid		= false;
	rockOn			= false;
	playerAttackData.hitDamage		= 0.0f;

	dist			= VZero;
	norm			= VZero;

	playerAttackData.frontSpeed		= 0.0f;

	rotation		= false;
	defalutTrail	= true;
	attackAgainStartCounter = 0.0f;
	playerAttackData.attackAgainStartCounterMax = 0.0f;

	playerAttackData.attackNum = 0;
	attackCount = 0;
	collsionCreate = false;

	playerAttackData.attackNum = 0;
	playerAttackData.attackAgainStartCounterMax = 0.0f;

	
}

PlayerAttackStateBase::~PlayerAttackStateBase()
{
}

void PlayerAttackStateBase::Update()
{
	Player* p = GetBase<Player>();
	SpecialAttackStart();
	if (p->playerCom.anim->AnimEventCan()) {
		if (defalutTrail) {
			p->playerCom.player->DrawTrail();
		}
		attackAgainStartCounter -= Time::DeltaTimeRate();
		if (attackAgainStartCounter <= 0.0f) {
			attackAgainStartCounter = playerAttackData.attackAgainStartCounterMax;
			AgainAttackCollsion();
		}
		/*p->playerCom.blur->MosionStart(0.04f, 0.1f, animId, 1);;*/
	}
	
	//p->RotationChange(p->GetWalkAngle(), 5.0f);
	//p->playerCom.player->DrawTrail();
	//å„åÑÇÃê›íË
	if (nextAvoid) {
		p->playerCom.player->AvoidReady();
		noStateChange = true;
		nextAvoid = false;
	}
	if (runTimer > 0.0f) {
		runTimer -= Time::DeltaTimeRate();
		//å„åÑÇ™èIÇÌÇ¡ÇΩÇÁéüÇÃèÛë‘Ç…ëJà⁄
		if (runTimer <= 0.0f) {
			if (nextAttack) {
				p->playerCom.stateManager->ChangeState(playerAttackData.normalAttackNextID);
			}
			
			else {
				/*if (id == ID::P_ANIM_JUST_AVOID_ATTACK5) {
					p->playerCom.player->AvoidFinishState();
				}
				else {
					p->playerCom.player->AvoidFinishState();
				}*/
				p->playerCom.player->AvoidFinishState();
			}
			
		}
	}
}

void PlayerAttackStateBase::EnemyRotation()
{
	if (!rockOn) {
		return;
	}
	Player* p = GetBase<Player>();
	//ìGÇ™àÍíËà»è„ÇÊÇËâìÇ¢Ç©âÒì]Ç™èIÇÌÇ¡ÇΩÇÁÇ±Ç±ÇÃèàóùÇÉXÉãÅ[Ç∑ÇÈ
	if (easingCount > 1.0f || !rotation) {
		return;
	}
	//ìGÇÃï˚å¸Ç…å¸ÇØÇÈ
	easingCount += p->playerCom.player->GetPlayerObj()->GetObjectTimeRate() * 6.0f;
	easingCount = min(1.0f, easingCount);

	p->playerCom.player->GetPlayerTransform()->rotation.y = Easing::Lerp(beforeAngle, angle, easingCount);
}

void PlayerAttackStateBase::Start()
{
	Player* p			= GetBase<Player>();
	//p->PlayerStickInput();
	nextAttack			= false;
	nextAvoid			= false;
	p->playerCom.player->SetAvoidStart(false);
	noStateChange		= false;

	if (p->playerCom.targetObj != nullptr) {
		p->attackTargetTrans		= *(p->playerCom.targetObj->GetTransform());
	}
	else {
		Transform nearEnemyPos = p->playerCom.enemyManager->NearEnemyPos(p->playerTransform->position);
		p->attackTargetTrans				= nearEnemyPos;
		//p->attackTargetTrans.position = VECTOR3(0, 0, 1) * p->playerTransform->rotation;
	}
	/*if (!p->playerCom.enemyManager->CameraInEnemy()) {
		p->playerCom.camera->AttackEnemyFovChange(&p->attackTargetTrans,1000.0f);
	}*/
	
	//AgainTimerSet(100.0f, 0);
	//ìGÇ∆ÉvÉåÉCÉÑÅ[ÇÃãóó£ÇÇ∆ÇÈ
	dist				= p->attackTargetTrans.position - p->playerCom.player->GetPlayerTransform()->position;
	
	VECTOR3 frontVector = VECTOR3(0.0f, 0.0f, 1.0f) * MGetRotY(p->playerTransform->rotation.y);
	rockOn = false;
	beforeAngle			= p->playerCom.player->GetPlayerTransform()->rotation.y;
	//if (VDot(dist, frontVector) >= 60.0f * DegToRad) {
	//	//äpìxåvéZ
	//	angle			= atan2f(dist.x, dist.z);
	//	rockOn			= true;
	//}
	//else {
	//	angle			= beforeAngle;
	//}
	angle = atan2f(dist.x, dist.z);
	rockOn			= true;
	
	easingCount			= 0.0f;
	firstColl			= true;
	distSize			= dist.Size();
	norm				= dist.Normalize();
	SoundManager::GetInstance()->RandamSe("P_AttackV", 3);
	beforeAttack		= true;
	runTimer			= 0.0f;
	/*attackAgainStartCounter = playerAttackData.attackAgainStartCounterMax;
	attackCount = playerAttackData.attackNum;*/
	attackCount = playerAttackData.attackNum;
	attackAgainStartCounter = playerAttackData.attackAgainStartCounterMax;
	//AgainTimerSet(playerAttackData.attackAgainStartCounterMax, playerAttackData.attackNum);
}

void PlayerAttackStateBase::Finish()
{
	Player* p = GetBase<Player>();
    p->DeleteCollision(&p->attackColl);
	if (!nextAttack) {
		p->playerCom.camera->CameraLeapSet(0.2f);
		p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	}
}

bool PlayerAttackStateBase::IsAttack()
{
	Player* p = GetBase<Player>();
	return p->playerCom.anim->AnimEventCan();
}

void PlayerAttackStateBase::AttackMoveStart()
{
	Player* p = GetBase<Player>();
	//if (dist.Size() >= 5000) {
		//ãóó£Ç™âìÇ¢Ç∆Ç‡Ç∆Ç‡Ç∆ÇÃäpìxÇ‘ÇÒçUåÇÇÃà⁄ìÆèàóùÇÇ¢ÇÍÇÈ
		//rotation = false;;
		//p->playerCom.physics->SetVelocity(VECTOR3(0, 0, frontSpeed) * MGetRotY(beforeAngle));
	//}
	//else {
		//ãﬂÇ¢Ç∆ìGÇÃï˚å¸Ç…å¸Ç©Ç¡ÇƒçUåÇÇÃà⁄ìÆèàóùÇÇ¢ÇÍÇÈ
		rotation = true;
		p->playerCom.physics->SetVelocity(VECTOR3(0, 0, playerAttackData.frontSpeed) * MGetRotY(angle));
	//}
}

void PlayerAttackStateBase::AgainAttackCollsion()
{
	Player* p = GetBase<Player>();

	if (attackCount <= 0) {
		return;
	}

	firstColl = true;
	attackCount--;

	/*if (attackCount % 2 == 0) {
		p->playerCom.anim->Play(animId, 0.01f);
		p->playerCom.anim->SetPlaySpeed(2.5f);
	}
	else {
		p->playerCom.anim->Play(ID::P_ANIM_ATTACK3, 0.01f);
		p->playerCom.anim->SetPlaySpeed(2.0f);
	}
	nextAttack = false;
	p->playerCom.sound->RandamSe("P_AttackV", 4);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);*/
	Debug::DebugLog("collsionCreate");
	p->DeleteCollision(&p->attackColl);
	firstColl = false;
	BaseAttackCollsion();
	/*p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("p_attack");

	ColliderBase* collider = p->obj->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;

	info.parentTransfrom = obj->GetTransform();
	info.oneColl = true;
	info.shape = CollsionInformation::SPHERE;
	info.tag = CollsionInformation::Tag::P_ATTACK;
	info.size = 1.0f;
	collider->CollsionAdd(info, collTrans, "p_attack");*/
}

void PlayerAttackStateBase::AgainTimerSet(float _time, int _attackNum)
{
	playerAttackData.attackNum = _attackNum;
	attackCount = playerAttackData.attackNum;
	attackAgainStartCounter = _time;
	playerAttackData.attackAgainStartCounterMax = attackAgainStartCounter;
}

void PlayerAttackStateBase::AttackCollsion()
{
	Player* p = GetBase<Player>();
	//ÉAÉjÉÅÅ[ÉVÉáÉìÇÃÉCÉxÉìÉgÇ™énÇ‹Ç¡ÇΩÇÁçUåÇÇÃìñÇΩÇËîªíËÇê∂ê¨Ç∑ÇÈÅB
	if (p->playerCom.anim->AnimEventCan()) {
		//ç≈èâÇÃàÍâÒÇÃÇ›ê∂ê¨ÇµÇΩÇ¢
		if (firstColl) {
			firstColl = false;
			BaseAttackCollsion();
			SoundManager::GetInstance()->RandamSe("swordWind", 5);
		}
	}
}

void PlayerAttackStateBase::SpecialAttackStart()
{
	Player* p = GetBase<Player>();
	if (InputManager::GetInstance()->KeyInputDown("SpecialAttack")) {
		if (p->CanSpecialAttack()) {
			p->playerCom.stateManager->ChangeState(StateID::PLAYER_SPECIAL_ATTACK_S);
			p->specialAttackBar = 0.0f;
		}

	}
}

void PlayerAttackStateBase::SetAttackData()
{
	Player* p = GetBase<Player>();
	playerAttackData = p->GetAttackDataMap(id);
}

void PlayerAttackStateBase::BaseAttackCollsion()
{
	Player* p = GetBase<Player>();
	p->playerCom.player->CollsionStart<SphereCollider>(&p->attackColl, playerAttackData.collTrans);
	p->playerCom.player->SetShape(CollsionInformation::SPHERE, &p->attackColl);
	
	collsionCreate = true;
}
