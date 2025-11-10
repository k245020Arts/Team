#pragma once
#include "../Library/GameObject.h"

class Transitor : public GameObject
{
public:

	enum TransitorType
	{
		STAY,
		PLAY,
		FINISH,
	};

	Transitor();
	~Transitor();

	virtual void StartTransitor(std::string _sceneName,float _time);

	virtual bool IsEnd() const;

protected:

	int newWindow;
	int oldWindow;

	float timeMax;
	float time;

	VECTOR2I size;

	TransitorType transitorType;

};