#pragma once
#include "Transitor.h"

class IrisTransitor : public Transitor
{
public:
	IrisTransitor();
	~IrisTransitor();

	void Update()override;
	void Draw()override;

	void SetMask();

	void StartIris(std::string _sceneName, float _time, bool _out);

private:
	int maskScreen;
	int maskHandle;
	float sizeCross;
	bool irisOut;
};
