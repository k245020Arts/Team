#include "GameManager.h"
#include "FadeTransitor.h"
#include "Object2D.h"
#include "MeshRenderer2D.h"
#include "ComponentManager.h"
#include "LoadManager.h"

namespace {
	const int GAME_STATE_MAX = 4;

	const std::string stateName[GAME_STATE_MAX]{
		{"BEFORE"},
		{"PLAY"},
		{"WIN"},
		{"LOSE"},
	};
}

GameManager::GameManager()
{
	state = MEB(&GameManager::BeforeUpdate);;
	stateDraw = MEBDraw(&GameManager::BeforeDraw);
	startCount = 4.0f;
	SetDrawOrder(-500);
}

GameManager::~GameManager()
{

}

void GameManager::Update()
{
	state = state.Act(this);
}

void GameManager::Draw()
{
	stateDraw.Act(this);
}

void GameManager::ChangeState(std::string _name)
{
	for (int i = 0; i < GAME_STATE_MAX;i++) {
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
				state = MEB(&GameManager::WinUpdate);
				stateDraw = MEBDraw(&GameManager::WinDraw);
				break;
			case 3:
				state = MEB(&GameManager::LoseUpdate);
				stateDraw = MEBDraw(&GameManager::LoseDraw);
				break;
			}
		}
	}
}

void GameManager::CreateNum()
{	
	obj = new Object2D();
	obj->Init(VECTOR2F(1000, 300), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.5f, 1.0f), "count");
	obj->SetDrawOrder(-500);

	MeshRenderer2D* mesh =  obj->Component()->AddComponent<MeshRenderer2D>();
	mesh->TextureHandle(Load::LoadImageGraph(Load::IMAGE_PATH + "Number_01", ID::START_NUM), MeshRenderer2D::DRAW_NUM);
}

MEB GameManager::BeforeUpdate()
{
	startCount -= Time::DeltaTime();
	obj->Component()->GetComponent<MeshRenderer2D>()->SetNum(startCount);
	return (startCount > 0.0f) ? &GameManager::BeforeUpdate : &GameManager::PlayUpdate;
}

MEB GameManager::BeforeDraw()
{
	//DrawFormatString(1000, 500, 0xffffff, "%d", (int)startCount);
	return &GameManager::BeforeDraw;
}

MEB GameManager::PlayUpdate()
{
	obj->Component()->GetComponent<MeshRenderer2D>()->SetDraw(false);
	return &GameManager::PlayUpdate;
}

MEB GameManager::PlayDraw()
{
	DrawFormatString(1000, 500, 0xffffff, "%d", (int)startCount);
	return &GameManager::PlayDraw;
}

MEB GameManager::WinUpdate()
{
	FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 2.0f);
	return MEB();
}

MEBDraw GameManager::WinDraw()
{
	return MEBDraw();
}

MEB GameManager::LoseUpdate()
{
	FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 0.1f);
	return MEB();
}

MEBDraw GameManager::LoseDraw()
{
	return MEBDraw();
}
