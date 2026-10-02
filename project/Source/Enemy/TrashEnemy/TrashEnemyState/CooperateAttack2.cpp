#include "CooperateAttack2.h"
#include "../TrashEnemy.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Collider/ColliderBase.h"
#include "../../../Common/Easing/Easing.h"
#include "../../../Common/InputManager/PadInput.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Component/Collider/SphereCollider.h"

CooperateAttack2::CooperateAttack2()
{
	string = Function::GetClassNameC<CooperateAttack2>();
	animId = ID::TE_R_C_ATTACK;
	attackParam.animID = ID::TE_R_C_ATTACK;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern = EnemyAttackBase::BACK;

	attackParam.useFlash = true;
	attackParam.attackFlashStartTime = 0.7f;
	attackParam.slowAmout = 0.1f;
	attackParam.slowTime = 0.3f;
	attackParam.speedUpMotionSpeed = 0.3f;

	damageMove = false;

	speedDownCounter = 0;

	hitStopCounter = 0.0f;

	pPos = VZero;

	isDamageMove = true;

	motionSpeed = 0;

	pCounter = 0;
}

CooperateAttack2::~CooperateAttack2()
{
}

void CooperateAttack2::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->IsPlayerSpecialMove())//•KŽE‹ZŽž‚ÉŽ~‚Ü‚ç‚¸‚É“®‚­‚©‚ç’âŽ~‚³‚¹‚é
		return;

	if (!damageMove)
			RangedMove(enemy);
		else
			DamageMove(enemy);
}

void CooperateAttack2::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	firstColl = true;
	attackParam.hitDamage = enemy->GetStatus().C_Attack2Damage;
	//enemy->enemyBaseComponent.camera->NowChangeStateCamera(StateID::R_ENEMY_CAMERA_S);
	//enemy->enemyBaseComponent.camera->CanNotStateChange();

	EnemyAttackBase::collTrans.position	= CollPos;
	EnemyAttackBase::collTrans.scale	= Collscale;

	enemy->isMovingToPlayer = true;
	pPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	pCounter = 0;

	EnemyStateBase::Start();
}

void CooperateAttack2::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	enemy->enemyBaseComponent.camera->CanStateChange();
	
	enemy->CooperateAtkFinish();
	enemy->enemyBaseComponent.camera->NowChangeStateCamera(StateID::FREE_CAMERA_S);
}

void CooperateAttack2::RangedMove(TrashEnemy* _enemy)
{
	const VECTOR3 enePos = _enemy->GetPos();
	const VECTOR3 targetPos = _enemy->cooperateWayPoint;
	VECTOR3 dir = VZero;
	const float Speed = 70.0f;
	const float SearchPosMax = 2000.0f;
	const float Max = 40.0f;
	
	if (VSize(pPos - enePos) > SearchPosMax && _enemy->isStandby)//‚Ç‚±‚Ü‚ÅƒvƒŒƒCƒ„[‚É’Ç]‚·‚é‚©
		pPos = _enemy->enemyBaseComponent.playerObj->GetTransform()->position;

	if (_enemy->cooperateDamageMove && isDamageMove)//ƒ_ƒ[ƒW‚ð‚à‚ç‚Á‚Ä‘Å‚¿•Ô‚³‚ê‚½‚Æ‚«‚Ìˆ—
	{
		_enemy->LookTarget(pPos);
		damageMove = true;
		_enemy->DeleteCollision(&_enemy->attackColl);
		InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartTime(ControllerPower, SceondTime);
		return;
	}
	else if (VSize(pPos - enePos) < Max )//’n–Ê‚É’…’n‚µ‚½Žž
	{
		if (!_enemy->cooperateDamageMove)//UŒ‚‚ðH‚ç‚Á‚Ä‚È‚©‚Á‚½‚Æ‚«
		{
			_enemy->enemyBaseComponent.anim->Play(ID::TE_R_IDOL);
			_enemy->enemyBaseComponent.anim->SetPlaySpeed(1.0f);//ƒ‚[ƒVƒ‡ƒ“‘¬“x‚ª0‚É‚È‚é‚©‚ç“ü‚ê‚é

			isDamageMove = false;
			_enemy->isStandby = false;
		}
		else if (!isDamageMove)//UŒ‚‚ðH‚ç‚¤‚Ì‚Æ’…’n‚ª“¯Žž‚¾‚Á‚½Žž
			isDamageMove = true;
		
		return;
	}
	else//“G‚ªUŒ‚‚µ‚½Œã‚Ì‹ì‚¯”²‚¯‹““®
	{

	}

	dir = VNorm(pPos - enePos);
	
	_enemy->GetEnemyObj()->GetTransform()->position += dir * Speed; 
	
	AttackCollsion();
	AttackSound();
	AttackFlash(ID::E_MODEL, 35, "E_AttackV");
	Trail();
}

void CooperateAttack2::DamageMove(TrashEnemy* _enemy)
{
	const float CounterMax = 1.0f;
	const float VecSpeed = 60.0f;
	//UŒ‚‚ðH‚ç‚Á‚½‚Æ‚«‚ÉƒJƒƒ‰‚ÌƒXƒe[ƒg‚ð•Ï‚¦‚é
	_enemy->enemyBaseComponent.camera->CanStateChange();
	_enemy->enemyBaseComponent.camera->ChangeStateCamera(StateID::R_ENEMY_FINISH_CAMERA_S);
	SoundManager::GetInstance()->PlaySe(Sound_ID::SOUND_ID::V_E_DAMAGE5);
	obj->Component()->RemoveAllComponent<SphereCollider>();

	if (pCounter == 0)
	{
		pPos = _enemy->enemyBaseComponent.playerObj->GetTransform()->position;
		pCounter = 1;
	}
	_enemy->enemyBaseComponent.playerObj->GetTransform()->position = pPos;

	hitStopCounter += Time::DeltaTimeRate();

	if (hitStopCounter < CounterMax)
	{
		_enemy->GetEnemyObj()->GetTransform()->position += sinf(hitStopCounter * VecSpeed) * VECTOR3(10, 0, 10);
		return;
	}
	
	const VECTOR3 enePos = _enemy->GetPos();
	const VECTOR3 targetPos = _enemy->cooperateWayPoint;
	const float Speed = 100.0f;

	VECTOR3 dir = VNorm(targetPos - enePos);
	
	speedDownCounter += Time::DeltaTimeRate();

	_enemy->GetEnemyObj()->GetTransform()->position += dir * (Speed - speedDownCounter);
}
