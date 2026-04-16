#include "T_EnemyDead.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Collider/ModelCollider.h"
#include "../../../Component/Collider/SphereCollider.h"
#include "../../../Common/Easing.h"
#include "../../../Component/Color/Color.h"
#include "../../EnemyManager.h"

T_EnemyDead::T_EnemyDead()
{
	animId = ID::E_DIE;
	string = Function::GetClassNameC<T_EnemyDead>();
	counter = 0;
	fadeCounter = FADE_SPEED;

}

T_EnemyDead::~T_EnemyDead()
{
}

void T_EnemyDead::Update()
{
	TrashEnemy* e = GetBase<TrashEnemy>();

	if (e->IsPlayerSpecialMove())
	{
		e->enemyBaseComponent.anim->SetPlaySpeed(0);
		return;
	}
	else
		e->enemyBaseComponent.anim->SetPlaySpeed(motionSpeed);

	KnockbackMove(e, e->deadPreset);

	if (e->enemyBaseComponent.anim->IsFinish() && e->GetEnemyObj()->GetTransform()->position.y <= 0)
	{
		fadeCounter -= Time::DeltaTimeRate();
		float reet = fadeCounter / FADE_SPEED;
		float alph = Easing::EaseIn(0.0f, 255.0f, reet);

		float color = 255.0f;
		e->enemyBaseComponent.color->setRGB(Color::Rgb(255, 255, 255, alph));
		if (fadeCounter <= 0)
			e->active = false;
	}
}

void T_EnemyDead::Draw()
{
}

void T_EnemyDead::Start()
{
	TrashEnemy* e = GetBase<TrashEnemy>();
	EnemyStateBase::Start();
	e->enemyBaseComponent.anim->SetPlaySpeed(0.8f);
	if (!e->IsPlayerSpecialMove())
	{
		obj->Component()->RemoveAllComponent<SphereCollider>();
		obj->Component()->RemoveAllComponent<ModelCollider>();
	}
	
	motionSpeed = e->enemyBaseComponent.anim->GetPlaySpeed();
	counter = 0;
	FindGameObject<EnemyManager>()->RemoveList(e, obj);
}

void T_EnemyDead::Finish()
{
}
