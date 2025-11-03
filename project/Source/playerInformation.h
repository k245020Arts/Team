#pragma once
#include "../Library/myDxLib.h"

class MeshRenderer;
class Physics;
class StateManager;
class Camera;
class InputManager;
class KeyboardInputManager;
class ControllerInputManager;
class Player;
class Animator;
class Color;
class BaseObject;
class Shaker;
class EffectManager;
class SoundManager;
class WeaponManager;
class MotionBlur;
class GameManager;
class EnemyManager;

namespace PlayerInformation {

	enum PlayerState
	{
		WAIT,
		WALK,
	};

	struct CharaComponent
	{
		CharaComponent() {
			stateManager = nullptr;
			renderer = nullptr;
			physics = nullptr;
			camera = nullptr;
			InputManager = nullptr;
			controller = nullptr;
			keyboard = nullptr;
			player = nullptr;
			anim = nullptr;
			color = nullptr;
			targetObj = nullptr;
			shaker = nullptr;
			effect = nullptr;
			sound = nullptr;
			weapon = nullptr;
			blur = nullptr;
			gameManager = nullptr;
			enemyManager = nullptr;
			hitObj = nullptr;
		}
		StateManager* stateManager;
		MeshRenderer* renderer;
		Physics* physics;
		Camera* camera;
		InputManager* InputManager;
		ControllerInputManager* controller;
		KeyboardInputManager* keyboard;
		Player* player;
		Animator* anim;
		Color* color;
		BaseObject* targetObj;
		BaseObject* hitObj;
		Shaker* shaker;
		EffectManager* effect;
		SoundManager* sound;
		WeaponManager* weapon;
		MotionBlur* blur;
		GameManager* gameManager;
		EnemyManager* enemyManager;
	};

	static const VECTOR3 BASE_INTERIA = VECTOR3(7500.0f, 2500.0f, 7500.0f);
	static const VECTOR3 BASE_GRAVITY = VECTOR3(0.0f, -1000.0f, 0.0f);
}
