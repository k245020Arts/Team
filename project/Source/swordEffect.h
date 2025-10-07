#pragma once
#include "color.h"
#include <vector>

class CharaWeapon;
class BaseObject;

class SwordEffect : public Component
{
public:
	SwordEffect();
	~SwordEffect();

	void Update()override;
	void Draw()override;
	void ImguiDraw()override;
	/// <summary>
	/// åïÇÃãOê’ÇÃê∂ê¨
	/// </summary>
	/// <param name="_nearPos">åïÇÃãOê’ÇÃññí[</param>
	/// <param name="_farPos">åïÇÃãOê’ÇÃêÊí[</param>
	/// <param name="_rgb">ãOê’ÇÃêF</param>
	/// <param name="_boneIndex">çú</param>
	/// <param name="_time">éûä‘</param>
	void CreateEffect(VECTOR3 _nearPos,VECTOR3 _farPos,Color::Rgb _rgb, int _boneIndex,float _time);

	VECTOR3 GetBezier(float t, VECTOR3 p0, VECTOR3 p1, VECTOR3 p2);

private:

	struct Info
	{
		Info() {
			time = 0.0f;
			weapon = nullptr;
			charaObj = nullptr;
			bone = -1;
			nearPos = VZero;
			farPos = VZero;
			midPos = VZero;
			rgb = Color::Rgb();
			count = 0;
			timeMax = 0.0f;
		}
		float time;
		float timeMax;

		CharaWeapon* weapon;
		BaseObject* charaObj;
		int bone;

		VECTOR3 nearPos;
		VECTOR3 farPos;
		VECTOR3 midPos;
		Color::Rgb rgb;

		int count;
	};

	

	int effectNum;
	int indexNum;

	static const int EFFECT_NUM = 100;
	static const int INDEX_NUM = (EFFECT_NUM - 2) * 3;;

	int image;

	std::vector<Info> num;

	void MakeDiv(std::vector<VERTEX3D>& vs, VECTOR rPos[4], VECTOR tPos[4], float v1, float v2,int i);

};
