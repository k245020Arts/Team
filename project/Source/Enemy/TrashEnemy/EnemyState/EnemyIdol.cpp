#include "EnemyIdol.h"
#include "EnemyStateManager.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../enemy.h"
#include "../../Common/Debug/Debug.h"
#include "../../GameManager/GameManager.h"

EnemyIdol::EnemyIdol()
{
	animId = ID::E_ANIM_IDOL;
	string = typeid(*this).name();
	const std::string prefix = "class ";
	if (string.find(prefix) == 0) {
		string = string.substr(prefix.length());
	}
	id = ID::E_ANIM_IDOL;
	timer = 0.0f;
}

EnemyIdol::~EnemyIdol()
{

}

void EnemyIdol::Update()
{
	Enemy* e = GetBase<Enemy>();
	if (e->enemyBaseComponent.gameManager->GetGameState() == &GameManager::BeforeUpdate) {
		return;
	}
	timer += Time::DeltaTime();
	//ˆê•b‚½‚Á‚½‚çUŒ‚
	if (timer >= 1.0f) {
		//Debug::DebugLogPrintf(Debug::printfString("timer = %.1f", timer));
		VECTOR3 sub = (EnemyInformation::BASE_POS ) - e->enemyBaseComponent.enemy->GetEnemyTransform()->position;
		float size = sub.Size();
		VECTOR3 base = EnemyInformation::BASE_POS + VECTOR3(500, 0, 0);
		float size2 = base.Size();
		if (size >= size2) {
			e->enemyBaseComponent.state->ChangeState(ID::E_RUN);
		}
		else {
			e->enemyBaseComponent.state->ChangeState(ID::E_ATTACK1);
		}
	}

	
}

void EnemyIdol::Draw()
{

}

void EnemyIdol::Start()
{
	timer = 0.0f;
	EnemyStateBase::Start();
}
