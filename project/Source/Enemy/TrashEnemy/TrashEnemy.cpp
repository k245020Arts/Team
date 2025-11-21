#include "TrashEnemy.h"

TrashEnemy::TrashEnemy()
{
	
}

TrashEnemy::~TrashEnemy()
{

}

void TrashEnemy::Update()
{
}

void TrashEnemy::Draw()
{
}

void TrashEnemy::CreateTrashEnemy(VECTOR3 _pos)
{
	obj->GetTransform()->position = _pos;
}
