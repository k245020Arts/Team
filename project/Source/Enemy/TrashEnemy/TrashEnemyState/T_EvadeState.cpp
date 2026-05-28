#include "T_EvadeState.h"
#include "../TrashEnemy.h"

T_EvadeState::T_EvadeState()
{
	animId = ID::TE_STANCE;
	string = Function::GetClassNameC<T_EvadeState>();
}

T_EvadeState::~T_EvadeState()
{
}

void T_EvadeState::Update()
{
}

void T_EvadeState::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

}

void T_EvadeState::Finish()
{
}
