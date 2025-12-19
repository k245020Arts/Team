#include "Transitor.h"
#include "../../Screen.h"

Transitor::Transitor()
{
	newWindow = -1;
	oldWindow = -1;
	time = -1;
	timeMax = -1;
	transitorType = STAY;
	DontDestroyOnSceneChange();
}

Transitor::~Transitor()
{
	if (oldWindow != -1) {
		DeleteGraph(oldWindow);
	}
	if (newWindow != -1) {
		DeleteGraph(newWindow);
	}
}

void Transitor::StartTransitor(std::string _sceneName, float _time)
{
	size.x = Screen::WIDTH;
	size.y = Screen::HEIGHT;

	if (newWindow != -1) {
		DeleteGraph(newWindow);
		newWindow = -1;
	}

	if (oldWindow != -1) {
		DeleteGraph(oldWindow);
		oldWindow = -1;
	}

	newWindow = MakeScreen(size.x, size.y, true);
	oldWindow = MakeScreen(size.x, size.y, true);

	int result = GetDrawScreenGraph(0, 0, size.x, size.y, oldWindow);
	time = _time;

	timeMax = _time;
	transitorType = PLAY;

	SetUseSetDrawScreenSettingReset(FALSE); //カメラのリセット

	SceneManager::ChangeScene(_sceneName);
}

bool Transitor::IsEnd() const
{
	return (time <= 0.0f);
}
