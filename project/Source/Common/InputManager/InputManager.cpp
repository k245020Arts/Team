#include "inputManager.h"
#include "../Debug/Debug.h"

InputManager::InputManager()
{
	controller = nullptr;
	keyboard = nullptr;
	mouse = nullptr;
	if (controller == nullptr)
	{
		controller = new PadInput();
	}
	if (keyboard == nullptr)
	{
		keyboard = new KeyboardInput();
	}
	if (mouse == nullptr)
	{
		mouse = new MouseInput();
	}

	//DontDestroyOnSceneChange(true);

	CreateInputData(KeyConfigData("attack", KEY_INPUT_E, XINPUT_BUTTON_B, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("avoid", KEY_INPUT_X, XINPUT_BUTTON_A, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("SceneChange", KEY_INPUT_P, XINPUT_BUTTON_A, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("debugChange", KEY_INPUT_Z, NOT_INPUT, MouseInput::RIGHT_CLICK));
	CreateInputData(KeyConfigData("camera", KEY_INPUT_SPACE, XINPUT_BUTTON_RIGHT_THUMB, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("changeMode", KEY_INPUT_TAB, -1, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("SpecialAttack", KEY_INPUT_Q, XINPUT_BUTTON_X, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("heavyAttack", KEY_INPUT_C, XINPUT_BUTTON_Y, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("Mouse", KEY_INPUT_F1, -1, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("playerParam", KEY_INPUT_LCONTROL, -1, MouseInput::MOUSE_NONE));

}

InputManager::~InputManager()
{
	inputData.clear();

	if (controller != nullptr)
	{
		delete controller;
	}
	if (keyboard != nullptr)
	{
		delete keyboard;
	}
	if (mouse != nullptr)
	{
		delete mouse;
	}
}

void InputManager::Update()
{
	if (controller != nullptr)
	{
		controller->Update();
	}
	if (keyboard != nullptr)
	{
		keyboard->Update();
	}
	if (mouse != nullptr)
	{
		mouse->Update();
	}
}

PadInput* InputManager::GetControllerInput()
{
	if (controller != nullptr)
	{
		return controller;
	}
	return nullptr;
}

KeyboardInput* InputManager::GetKeyboardInput()
{
	if (keyboard != nullptr)
	{
		return keyboard;
	}
	return nullptr;
}

MouseInput* InputManager::GetMouseInput()
{
	if(mouse!=nullptr)
	{
		return mouse;
	}
	return nullptr;
}

void InputManager::CreateInputData(const KeyConfigData& _data)
{
	auto it = inputData.find(_data.action);
	if (inputData.end() == it) {
		inputData[_data.action] = _data;
	
	}
	else {
		Debug::DebugLog("そのInputDataは存在しています" + _data.action);
	}
}

bool InputManager::KeyInputDown(std::string _name)
{
	bool data = IsData(_name);
	if (!data) {//データが存在していないならキーを入力をしていないものとして返す
		Debug::DebugLog("noAction");
		return false;
	}
	PadInput* c = GetControllerInput();
	KeyboardInput* k = GetKeyboardInput();
	MouseInput* m = GetMouseInput();
	bool control = false;
	bool key = false;
	bool mouse = false;

	if (c != nullptr) {
		control = c->GetIsButtonPutNow(inputData[_name].padNumber);
	}
	if (k != nullptr) {
		key = k->GetIsKeyboardPut(inputData[_name].keyNumber);
	}
	if (m != nullptr) {
		mouse = m->GetMousePut(inputData[_name].mouceNumber);
	}
	if (control || key || mouse) {
		return true;
	}
	return false;
}

bool InputManager::KeyInput(std::string _name)
{
	bool data = IsData(_name);
	if (!data) { //データが存在していないならキーを入力をしていないものとして返す
		Debug::DebugLog("noAction");
		return false;
	}
	PadInput* c = GetControllerInput();
	KeyboardInput* k = GetKeyboardInput();
	MouseInput* m = GetMouseInput();
	bool control = false;
	bool key = false;
	bool mouse = false;

	if (c != nullptr) {
		control = c->GetIsButtonPushingNow(inputData[_name].padNumber);
	}
	if (k != nullptr) {
		key = k->GetIsKeyboardPushing(inputData[_name].keyNumber);
	}
	if (m != nullptr) {
		mouse = m->GetMousePushing(inputData[_name].mouceNumber);
	}
	if (control || key || mouse) {
		return true;
	}
	return false;
}

bool InputManager::KeyInputUp(std::string _name)
{
	bool data = IsData(_name);
	if (!data) {//データが存在していないならキーを入力をしていないものとして返す
		Debug::DebugLog("noAction");
		return false;
	}
	PadInput* c = GetControllerInput();
	KeyboardInput* k = GetKeyboardInput();
	MouseInput* m = GetMouseInput();
	bool control = false;
	bool key = false;
	bool mouse = false;

	if (c != nullptr) {
		control = c->GetIsButtonReleaseNow(inputData[_name].padNumber);
	}
	if (k != nullptr) {
		key = k->GetIsKeyboardRelease(inputData[_name].keyNumber);
	}
	if (m != nullptr) {
		mouse = m->GetMousePut(inputData[_name].mouceNumber);
	}
	if (control || key || mouse) {
		return true;
	}
	return false;
}

bool InputManager::IsData(std::string _name)
{
	auto it = inputData.find(_name);
	if (inputData.end() == it) {//データが存在していないならfalse
		return false;
	}
	else {
		return true;
	}
}

/*bool InputManager::GetIsKeyOrButtonPushingNow(int _key, int _button)
{
	if(GetIsKeyPushingNow(_button)||GetIsKeyboardPushing(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::GetIsKeyOrButtonPutNow(int _key, int _button)
{
	if (GetIsKeyPutNow(_button) || GetIsKeyboardPut(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::GetIsKeyOrButtonReleaseNow(int _key, int _button)
{
	if (GetIsKeyReleaseNow(_button) || GetIsKeyboardRelease(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}*/