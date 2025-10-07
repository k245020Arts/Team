#pragma once
#include "../Library/GameObject.h"
#include "MethodExec.h"
#include "Object2D.h"

typedef MethodExec<class GameManager> MEB;
typedef MethodExec<class GameManager> MEBDraw;

class GameManager : public GameObject
{
public:

	friend class EnemyIdol;
	friend class PlayerWait;
	friend class PlayerDie;
	friend class EnemyDie;
	
	GameManager();
	~GameManager();

	void Update()override;
	void Draw()override;

	void ChangeState(std::string _name);

	MEB::MethodPtr GetGameState() { return state.GetMethodFunc(); }

	void CreateNum();

private:
	
	MEB state;
	MEBDraw stateDraw;

	MEB BeforeUpdate();
	MEBDraw BeforeDraw();

	MEB PlayUpdate();
	MEBDraw PlayDraw();

	MEB WinUpdate();
	MEBDraw WinDraw();

	MEB LoseUpdate();
	MEBDraw LoseDraw();

	float startCount;
	Object2D* obj;
};