#include "GameManager.h"
#include "../Common/Transitor/FadeTransitor.h"
#include "../Component/Object/Object2D.h"
#include "../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../Component/ComponentManager.h"
#include "../Common/LoadManager.h"
#include "../Common/Sound/SoundManager.h"
#include "../Camera/Camera.h"
#include "../Wave/Wave.h"
#include "../../ImGui/imgui.h"
#include "../Common/Debug/Debug.h"

namespace {
	const int GAME_STATE_MAX = 5;

	const std::string stateName[GAME_STATE_MAX]{
		{"BEFORE"},
		{"PLAY"},
		{"BOSS_PLAY_BEFORE"},
		{"WIN"},
		{"LOSE"},
	};
}



GameManager::GameManager()
{
	state = MEB(&GameManager::BeforeUpdate);;
	stateDraw = MEBDraw(&GameManager::BeforeDraw);
	startCount = 0.0f;
	SetDrawOrder(-500);
	winImage = Load::LoadImageGraph(Load::IMAGE_PATH + "Win", ID::WIN);
	loseImage = Load::LoadImageGraph(Load::IMAGE_PATH + "Lose", ID::LOSE);
	resultCounter = 3.0f;
	sound = FindGameObject<SoundManager>();

#ifdef STRING_MODE
	beforeState = "";
	nowState = "BEFORE";
#else
	beforeGameState = GameState::NONE;
	gameState = GameState::BEFORE;
#endif // STRING_MODE
	
	changeState = true;
	
}

GameManager::~GameManager()
{

}

void GameManager::Update()
{
	if (changeState) {
		changeState = false;
	}
	else {

#ifdef STRING_MODE
		if (nowState != beforeState) {
			beforeState = nowState;
			changeState = true;
		}
#else
		if (gameState != beforeGameState) {
			beforeGameState = gameState;
			changeState = true;
		}
#endif

	}
	state = state.Act(this);

#ifdef STRING_MODE
	if (Debug::GetDebugMode()) {
		ImGui::Begin("gameState");
		if (ImGui::Button("before")) {
			ChangeState("BEFORE");
		}
		if (ImGui::Button("play")) {
			ChangeState("PLAY");
		}
		if (ImGui::Button("win")) {
			ChangeState("WIN");
		}
		if (ImGui::Button("lose")) {
			ChangeState("LOSE");
		}
		ImGui::End();
	}
#else
	if (Debug::GetDebugMode()) {
		ImGui::Begin("gameState");
		if (ImGui::Button("before")) {
			ChangeState(GameState::BEFORE);
		}
		if (ImGui::Button("play")) {
			ChangeState(GameState::PLAY);
		}
		if (ImGui::Button("win")) {
			ChangeState(GameState::WIN);
		}
		if (ImGui::Button("lose")) {
			ChangeState(GameState::LOSE);
		}
		ImGui::End();
	}
#endif // STRING_MODE

	
	
}

void GameManager::Draw()
{
	stateDraw.Act(this);
}

#ifdef STRING_MODE

void GameManager::ChangeState(std::string _name)
{
	for (int i = 0; i < GAME_STATE_MAX; i++) {
		if (stateName[i] == _name) {
			switch (i)
			{
			case 0:
				state = MEB(&GameManager::BeforeUpdate);
				stateDraw = MEBDraw(&GameManager::BeforeDraw);
				break;
			case 1:
				state = MEB(&GameManager::PlayUpdate);
				stateDraw = MEBDraw(&GameManager::PlayDraw);
				break;
			case 2:
				state = MEB(&GameManager::BossPlayBeforeUpdate);
				stateDraw = MEBDraw(&GameManager::BossPlayBeforeDraw);
				break;
			case 3:
				state = MEB(&GameManager::WinUpdate);
				stateDraw = MEBDraw(&GameManager::WinDraw);

				break;
			case 4:
				state = MEB(&GameManager::LoseUpdate);
				stateDraw = MEBDraw(&GameManager::LoseDraw);
				break;
			}

			nowState = stateName[i];
			//changeState = true;
		}
	}
}


int GameManager::GetStateNumber()
{
	int num = 0;
	if (nowState == "BEFORE") {
		num = 0;
	}
	else if (nowState == "PLAY") {
		num = 1;
	}
	else if (nowState == "BOSS_PLAY_BEFORE") {

		num = 2;
	}
	else if (nowState == "WIN") {
		num = 3;
	}
	else {
		num = 4;
	}
	return num;
}


std::string GameManager::GetStateName()
{
	return nowState;
}

#else

void GameManager::ChangeState(GameState _name)
{
	gameState = _name;

	switch (_name)
	{
	case GameState::BEFORE:
		state = MEB(&GameManager::BeforeUpdate);
		stateDraw = MEBDraw(&GameManager::BeforeDraw);
		break;

	case GameState::PLAY:
		state = MEB(&GameManager::PlayUpdate);
		stateDraw = MEBDraw(&GameManager::PlayDraw);
		break;

	case GameState::BOSS_PLAY_BEFORE:
		state = MEB(&GameManager::BossPlayBeforeUpdate);
		stateDraw = MEBDraw(&GameManager::BossPlayBeforeDraw);
		break;

	case GameState::WIN:
		state = MEB(&GameManager::WinUpdate);
		stateDraw = MEBDraw(&GameManager::WinDraw);
		break;

	case GameState::LOSE:
		state = MEB(&GameManager::LoseUpdate);
		stateDraw = MEBDraw(&GameManager::LoseDraw);
		break;
	case GameState::SCENE_CHANGE:
		state = MEB(&GameManager::SceneChangeUpdate);
		stateDraw = MEBDraw(&GameManager::SceneChangeDraw);
		break;
	default:
		break;
	}
}

GameManager::GameState GameManager::GetStateNumber()
{
	return gameState;
}

#endif // STRING_MODE



void GameManager::SetPointer()
{
	camera = FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>();
}

MEB GameManager::BeforeUpdate()
{
	//startCount -= Time::DeltaTime();
	//obj->Component()->GetComponent<MeshRenderer2D>()->SetNum((int)startCount);
	bool isCutScene = (camera->IsCutScene());
	if (isCutScene) {
		return &GameManager::BeforeUpdate;
	}
	else {
		FindGameObject<Wave>()->FirstRespown();
#ifdef STRING_MODE
		ChangeState("PLAY");
#else
		ChangeState(GameState::PLAY);
#endif // STRING_MODE

		
		return &GameManager::PlayUpdate;
	}
}

MEB GameManager::BeforeDraw()
{
	//DrawFormatString(1000, 500, 0xffffff, "%d", (int)startCount);
	return &GameManager::BeforeDraw;
}

MEB GameManager::PlayUpdate()
{
	//obj->Component()->GetComponent<MeshRenderer2D>()->SetDraw(false);
	return &GameManager::PlayUpdate;
}

MEB GameManager::PlayDraw()
{
	//DrawFormatString(1000, 500, 0xffffff, "%d", (int)startCount);
	return &GameManager::PlayDraw;
}

MEB GameManager::BossPlayBeforeUpdate()
{
	return  &GameManager::BossPlayBeforeUpdate;
}

MEB GameManager::BossPlayBeforeDraw()
{
	return &GameManager::BossPlayBeforeDraw;
}

MEB GameManager::WinUpdate()
{

	//FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 1.0f);
	return &GameManager::WinUpdate;
}

MEBDraw GameManager::WinDraw()
{
	
	DrawGraph(750, 100, winImage, true);
	return MEBDraw();
}

MEB GameManager::LoseUpdate()
{
	return &GameManager::LoseUpdate;
}

MEBDraw GameManager::LoseDraw()
{
	
	DrawGraph(750, 100, loseImage, true);
	return &GameManager::LoseDraw;
}

MEB GameManager::SceneChangeUpdate()
{
	FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 1.0f);
	return  &GameManager::SceneChangeUpdate;
}

MEBDraw GameManager::SceneChangeDraw()
{
	return  &GameManager::SceneChangeDraw;
}
