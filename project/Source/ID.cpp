#include "ID.h"
#include <unordered_map>
#include <assert.h>
#include "Debug.h"

namespace {
	std::unordered_map<int,std::string> modelId;

	std::string InitID[ID::ID_MAX] = {
		"enemy",
		"player",
		"stage",
		"p_idol",
		"p_run",
		"p_avoid",
		"p_just_avoid",
		"p_attack1",
		"p_just_avoid_attack1",
		"p_attack2",
		"p_just_avoid_attack2",
		"p_attack3",
		"p_just_avoid_attack3",
		"p_attack4",
		"p_just_avoid_attack4",
		"p_just_avoid_attack5",
		"p_attack5",
		"p_damage",
		"p_blowAway",
		"p_fall",
		"p_getUp",
		"p_weapon",
		"e_idol",
		"e_attack",
		"e_damage",
		"e_blowaway",
		"e_fall",
		"e_getUp",
		"e_weapon",
		"c_follow",
		"c_avoid_attack",
		"e_run",
		"c_hit",
		"sky",
		"swordLoad",
		"wall",
		"p_hp_guage",
		"e_die",
		"p_die",
		"start_num",
		"b_model",
		"c_free",
		"b_idol"
		"b_run"
		"b_coolTime",//
		"b_attackSorting"//
		"b_n_attack1",
		"b_n_attack2",
		"b_n_attack3",
		"b_n_attack4",
		"b_n_attack5",
		"b_s_attack1",
		"b_s_attack2",
		"b_s_attack3",
		"b_s_attack4",
		"sword_effect_b",
		"bossPush",
	};
}

void ID::Init() {

	for (int i = 0; i < ID_MAX; i++) {
		modelId[i] = InitID[i];
	}
}

std::string ID::GetID(IDType _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)ID_MAX) {
		Debug::CreateMessageBox("modelHandleMiss", "miss");
	}
	return modelId[type];
}