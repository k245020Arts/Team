#include "PlayerAttackStateBase.h"
#include "player.h"
#include "Easing.h"
#include "SoundManager.h"
#include "playerStateManager.h"
#include "Physics.h"
#include "weaponManager.h"
#include "Animator.h"
#include "MotionBlur.h"

PlayerAttackStateBase::PlayerAttackStateBase()
{
	easingCount = 0.0f;
	beforeAngle = 0.0f;
	angle = 0.0f;
	nextAttack = 0.0f;
	distSize = 0.0f;
	frontSpeed = 0.0f;
	nextAttackID = ID::P_ANIM_ATTACK1;
	rotation = 0.0f;
	runTimer = 0.0f;
	time = 0.0f;
	beforeAngle = 0.0f;
	beforeAttack = false;
}

PlayerAttackStateBase::~PlayerAttackStateBase()
{
}

void PlayerAttackStateBase::Update()
{
	Player* p = GetBase<Player>();
	if (p->playerCom.anim->AnimEventCan()) {
		p->playerCom.player->DrawTrail();
		/*p->playerCom.blur->MosionStart(0.04f, 0.1f, animId, 1);;*/
	}
	//p->playerCom.player->DrawTrail();
	//å„åÑÇÃê›íË
	if (runTimer > 0.0f) {
		runTimer -= Time::DeltaTimeRate();
		//å„åÑÇ™èIÇÌÇ¡ÇΩÇÁéüÇÃèÛë‘Ç…ëJà⁄
		if (runTimer <= 0.0f) {
			if (nextAttack) {
				p->playerCom.stateManager->ChangeState(nextAttackID);
			}
			else if (nextAvoid) {
				p->playerCom.player->AvoidReady();
				noStateChange = true;
			}
			else {
				if (id == ID::P_ANIM_JUST_AVOID_ATTACK5) {
					p->playerCom.player->AvoidFinishState();
				}
				else {
					p->playerCom.stateManager->ChangeState(ID::P_ANIM_RUN);
				}
			}
			
		}
	}
}

void PlayerAttackStateBase::EnemyRotation()
{
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
	Player* p = GetBase<Player>();
	nextAttack = false;
	nextAvoid = false;
	p->playerCom.player->SetAvoidStart(false);
	noStateChange = false;
	if (p->playerCom.targetObj != nullptr) {
		targetTrans = *(p->playerCom.targetObj->GetTransform());
	}
	else {
		targetTrans = Transform();
	}
	
	//ìGÇ∆ÉvÉåÉCÉÑÅ[ÇÃãóó£ÇÇ∆ÇÈ
	dist = targetTrans.position - p->playerCom.player->GetPlayerTransform()->position;
	//äpìxåvéZ
	angle = atan2f(dist.x, dist.z);
	easingCount = 0.0f;
	beforeAngle = p->playerCom.player->GetPlayerTransform()->rotation.y;
	firstColl = true;
	distSize = dist.Size();
	norm = dist.Normalize();
	p->playerCom.sound->RandamSe("P_AttackV", 3);
	beforeAttack = true;
	runTimer = 0.0f;
	if (dist.Size() >= 2500 && p->playerCom.targetObj != nullptr) {
		//ãóó£Ç™âìÇ¢Ç∆Ç‡Ç∆Ç‡Ç∆ÇÃäpìxÇ‘ÇÒçUåÇÇÃà⁄ìÆèàóùÇÇ¢ÇÍÇÈ
		rotation = false;;
		p->playerCom.physics->SetVelocity(VECTOR3(0, 0, frontSpeed) * MGetRotY(beforeAngle));
	}
	else {
		//ãﬂÇ¢Ç∆ìGÇÃï˚å¸Ç…å¸Ç©Ç¡ÇƒçUåÇÇÃà⁄ìÆèàóùÇÇ¢ÇÍÇÈ
		rotation = true;
		p->playerCom.physics->SetVelocity(VECTOR3(0, 0, frontSpeed) * MGetRotY(angle));
	}
	
}

bool PlayerAttackStateBase::IsAttack()
{
	Player* p = GetBase<Player>();
	return p->playerCom.anim->AnimEventCan();
}
