#pragma once
#include "../../Library/GameObject.h"
#include "../Common/MethodExec.h"
#include "../Component/Object/Object2D.h"

typedef MethodExec<class GameManager> MEB;
typedef MethodExec<class GameManager> MEBDraw;
class SoundManager;
class Camera;

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

	std::string GetStateName();

	void SetPointer();

	bool GetChangeState() { return changeState; }

	int GetStateNumber();

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

	int winImage;
	int loseImage;
	float resultCounter;
	SoundManager* sound;

	std::string nowState;
	Camera* camera;
	bool changeState;
};