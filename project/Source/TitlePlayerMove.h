#pragma once
#include "TitlePlayer.h"
#include "Player/PlayerState/PlayerStateBase.h"

class TitlePlayerMove : public PlayerStateBase
{
public:
	TitlePlayerMove();
	~TitlePlayerMove();
	void Update() override;
	void Draw() override;
	void Start() override;
	void Finish()override;

private:

};