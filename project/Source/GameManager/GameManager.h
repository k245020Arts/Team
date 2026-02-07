#pragma once
#include "../../Library/GameObject.h"
#include "../Common/MethodExec.h"
#include "../Component/Object/Object2D.h"

typedef MethodExec<class GameManager> MEB;
typedef MethodExec<class GameManager> MEBDraw;
class SoundManager;
class Camera;

//#define STRING_MODE

class GameManager : public GameObject
{
public:

	enum GameState
	{
		NONE = -1,
		BEFORE,
		PLAY,
		BOSS_PLAY_BEFORE,
		WIN,
		LOSE,
		SCENE_CHANGE,
	};

	friend class EnemyIdol;
	friend class PlayerWait;
	friend class PlayerDie;
	friend class EnemyDie;
	
	GameManager();
	~GameManager();

	void Update()override;
	void Draw()override;

#ifdef STRING_MODE
	void ChangeState(std::string _name);
	//std::string GetStateName();
	int GetStateNumber();
#else
	GameState GetStateNumber();
	void ChangeState(GameState _name);
#endif // STRING_MODE

	

	

	MEB::MethodPtr GetGameState() { return state.GetMethodFunc(); }

	void CreateNum();

	
	

	void SetPointer();

	bool GetChangeState() { return changeState; }

	

private:
	
	MEB state;
	MEBDraw stateDraw;

	MEB BeforeUpdate();
	MEBDraw BeforeDraw();

	MEB PlayUpdate();
	MEBDraw PlayDraw();

	MEB BossPlayBeforeUpdate();
	MEB BossPlayBeforeDraw();

	MEB WinUpdate();
	MEBDraw WinDraw();

	MEB LoseUpdate();
	MEBDraw LoseDraw();

	MEB SceneChangeUpdate();
	MEBDraw SceneChangeDraw();

	float startCount;
	Object2D* obj;

	int winImage;
	int loseImage;
	float resultCounter;
	SoundManager* sound;

	std::string nowState;
	Camera* camera;
	bool changeState;
	std::string beforeState;

	GameState gameState;
	GameState beforeGameState;
};