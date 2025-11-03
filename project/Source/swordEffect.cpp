#include "swordEffect.h"
#include "LoadManager.h"
#include "CharaWeapon.h"
#include "ComponentManager.h"
#include "transform.h"
#include "../ImGui/imgui.h"
#include <array>
#include "Easing.h"


SwordEffect::SwordEffect()
{
	
	Load::LoadImageGraph(Load::IMAGE_PATH + "zlRYsodZuqpGepk1757160129_1757160135", ID::SWORD_EFFECT_B);
	Load::LoadImageGraph(Load::IMAGE_PATH + "photo_12", ID::SWORD_EFFECT);
	num.clear();
	image = -1;
	effectNum = 0;
	indexNum = 0;
	debugId = 18;
	tag = Function::GetClassNameC<SwordEffect>();
}

SwordEffect::~SwordEffect()
{
}

void SwordEffect::Update()
{
	for (auto e = num.begin(); e != num.end();) {
		if (e->feedInTime > 0.0f) {
			e++;
			continue;
		}
		e->time -= Time::DeltaTimeRate();
		if (e->time <= 0.0f) {
			e->feedOutTime -= Time::DeltaTimeRate();
			if (e->feedOutTime <= 0.0f) {
				e = num.erase(e);
			}
		}
		else {
			e++;
		}
	}
}

void SwordEffect::Draw()
{
	
	if (num.size() < 2) {
		return;
	}

	std::vector<VERTEX3D> vertexes;			// 頂点バッファ
	std::vector<unsigned short> indexes;	// インデックスバッファ
	float vdif = 1.0f / (num.size() - 1);
	for (int i = 0; i < num.size() - 1; i++) {

		// 補間しながら頂点をつくる
		VECTOR3 rPos[4]; // nearPosの補間用の４頂点
		rPos[0] = (i == 0) ? num[0].nearPos : num[i - 1].nearPos;
		rPos[1] = num[i].nearPos;
		rPos[2] = num[i + 1].nearPos;
		rPos[3] = (i == num.size() - 2) ? num[num.size() - 1].nearPos : num[i + 2].nearPos;

		VECTOR tPos[4]; // farPosの補間用の４頂点
		tPos[0] = (i == 0) ? num[0].farPos : num[i - 1].farPos;
		tPos[1] = num[i].farPos;
		tPos[2] = num[i + 1].farPos;
		tPos[3] = (i == num.size() - 2) ? num[num.size() - 1].farPos : num[i + 2].farPos;

		float v0 = (float)num[i].time / num[i].timeMax;
		float v1 = (float)num[i + 1].time / num[i].timeMax;

		MakeDiv(vertexes, rPos, tPos, v0, v1,i);
	}

	//インデックスの設定
	for (int i = 0; i < (vertexes.size() - 2) / 2; i++) {
		indexes.push_back(i * 2);
		indexes.push_back(i * 2 + 1);
		indexes.push_back(i * 2 + 2);
		indexes.push_back(i * 2 + 2);
		indexes.push_back(i * 2 + 1);
		indexes.push_back(i * 2 + 3);
	}

	SetUseZBuffer3D(FALSE);
	SetUseLighting(FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	int indexSize = static_cast<int>(indexes.size());
	int vertexSize = static_cast<int>(vertexes.size());

	if (indexSize >= 3)
		DrawPolygonIndexed3D(&vertexes[0], vertexSize, &indexes[0], indexSize / 3, image, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);

}

void SwordEffect::ImguiDraw()
{
	ImGui::Text("%d", num.size());
}

void SwordEffect::CreateEffect(VECTOR3 _nearPos, VECTOR3 _farPos, Color::Rgb _rgb, int _boneIndex,float _time)
{
	Info in;

	in.weapon = obj->Component()->GetComponent<CharaWeapon>();
	in.charaObj = in.weapon->GetChara();
	in.bone = _boneIndex;

	VECTOR3 pos = MV1GetFramePosition(in.weapon->CharaModel(), in.bone);

	in.nearPos = _nearPos *  in.weapon->GetWeaponMatrix();
	in.farPos = _farPos * in.weapon->GetWeaponMatrix();
	in.midPos = (_nearPos + _farPos) / 2.0f;
	in.rgb = _rgb;
	//image = Load::GetHandle(ID::SWORD_EFFECT);

	in.count = 0;
	
	//time = _time;
	in.timeMax = _time;
	in.time = _time;
	in.maxAlpha = _rgb.a;
	in.feedInTime = 0.2f;
	in.feedOutTime = 0.5f;
	num.emplace_back(in);
}

void SwordEffect::SetImage(int _model)
{
	image = _model;
}

VECTOR3 SwordEffect::GetBezier(float t, VECTOR3 p0, VECTOR3 p1, VECTOR3 p2)
{
	float u = 1.0f - t;
    return u * u * p0 + 2 * u * t * p1 + t * t * p2;

}

void SwordEffect::MakeDiv(std::vector<VERTEX3D>& vs, VECTOR rPos[4], VECTOR tPos[4], float v1, float v2,int i)
{
	const int divNum = 4;

	// root補間の変数を作る
	VECTOR rV0 = (rPos[2] - rPos[0]) / 2.0f;
	VECTOR rV1 = (rPos[3] - rPos[1]) / 2.0f;
	VECTOR rPar1 = 2 * rPos[1] - 2 * rPos[2] + rV0 + rV1;
	VECTOR rPar2 = -3 * rPos[1] + 3 * rPos[2] - 2 * rV0 - rV1;

	// tip補間の変数を作る
	VECTOR tV0 = (tPos[2] - tPos[0]) / 2.0f;
	VECTOR tV1 = (tPos[3] - tPos[1]) / 2.0f;
	VECTOR tPar1 = 2 * tPos[1] - 2 * tPos[2] + tV0 + tV1;
	VECTOR tPar2 = -3 * tPos[1] + 3 * tPos[2] - 2 * tV0 - tV1;

	COLOR_U8 spcColor;
	spcColor.r = (BYTE)num[i].rgb.r;
	spcColor.g = (BYTE)num[i].rgb.g;
	spcColor.b = (BYTE)num[i].rgb.b;
	//spcColor.a = (BYTE)num[i].rgb.a;
	
	//フェードインとフェードアウト
	if (num[i].feedInTime > 0.0f) {
		num[i].feedInTime -= Time::DeltaTimeRate();
		if (num[i].feedInTime <= 0.0f) {
			num[i].feedInTime = 0.0f;
		}
		float rate = 1 - (num[i].feedInTime / 0.2f);
		spcColor.a =  Easing::Lerp(0.0f, num[i].maxAlpha, rate);
	}
	else if (num[i].time <= 0.0f) {
		float rate = (num[i].feedOutTime / 0.5f);
		spcColor.a = Easing::Lerp(0.0f, num[i].maxAlpha, rate);
	}
	else {
		spcColor.a = num[i].maxAlpha;
	}

	VERTEX3D v;
	v.norm = VECTOR3(0, 1, 0);
	v.dif = spcColor;
	v.spc = spcColor;

	

	for (int i = 0; i < divNum; i++) {
		float t = (float)i / divNum;
		float t2 = t * t;
		float t3 = t2 * t;

		// rootの補間をしてpush_back
		v.pos = rPar1 * t3 + rPar2 * t2 + rV0 * t + rPos[1];
		v.u = 0.0f; v.v = (v2 - v1) * t + v1;
		vs.push_back(v);

		// tipの補間をしてpush_back
		v.pos = tPar1 * t3 + tPar2 * t2 + tV0 * t + tPos[1];
		v.u = 1.0f; v.v = (v2 - v1) * t + v1;
		vs.push_back(v);
	}
}
