#pragma once

class StateManager;
class Enemy;
class Animator;
class Physics;
class ControllerInputManager;
class Color;
class Shaker;
class BaseObject;
class Camera;
class EffectManager;
class SoundManager;
class WeaponManager;
class GameManager;

namespace EnemyInformation {

	struct EnemyComponent
	{
		EnemyComponent() {
			state = nullptr;
			enemy = nullptr;
			physics = nullptr;
			camera = nullptr;
			control = nullptr;
			playerObj = nullptr;
			anim = nullptr;
			color = nullptr;
			shaker = nullptr;
			effect = nullptr;
			sound = nullptr;
			weapon = nullptr;
			gameManager = nullptr;
		}
		StateManager* state;
		Enemy* enemy;
		Physics* physics;
		Animator* anim;
		ControllerInputManager* control;
		Color* color;
		Shaker* shaker;
		BaseObject* playerObj;
		Camera* camera;
		EffectManager* effect;
		SoundManager* sound;
		WeaponManager* weapon;
		GameManager* gameManager;
	};

	//ç≈èâÇÃÉ|ÉWÉVÉáÉì
	const VECTOR3 BASE_POS = VECTOR3(500, 1000, 0);

};