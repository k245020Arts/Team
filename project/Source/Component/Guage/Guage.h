#pragma once
#include "../Component.h"
#include "../ComponentManager.h"
#include "../../CharaBase/CharaBase.h"
#include "../MeshRenderer2D/MeshRenderer2D.h"

class Guage : public Component
{
public:

	enum BAR_MODE
	{
		HP,
		SPECIAL_ATTACK,
	};
	Guage();
	~Guage();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// HPƒQ[ƒW‚ÌHp‚ğæ“¾
	/// </summary>
	/// <typeparam name="T">’N‚ÌHP‚ğæ“¾‚µ‚½‚¢‚©</typeparam>
	template <typename T>
	void GetHp() {
		barValue = obj->GetParent()->Component()->GetComponent<T>()->GetHp();
		barValueMax = obj->GetParent()->Component()->GetComponent<T>()->GetMaxHp();
	}

	template <typename T>
	void GetSpecialAttack() {
		barValue = obj->GetParent()->Component()->GetComponent<T>()->GetSpecialAttackGuage();
		barValueMax = obj->GetParent()->Component()->GetComponent<T>()->GetMaxSpecialAttackGuage();
	}

	/// <summary>
	/// HPƒo[‚Ì˜g‚ğ•`‰æ
	/// </summary>
	/// <param name="_image"></param>
	/// <param name="_mode"></param>
	/// <param name="_transfrom"></param>
	void EdgeDrawReady(int _image, MeshRenderer2D::GraphMode _mode,Transform _transfrom);
	template <typename T>
	void GuageDrawReady(int _image, MeshRenderer2D::GraphMode _mode,BAR_MODE _bar_mode) {
		guage = obj->Component()->AddComponent<MeshRenderer2D>();
		guage->TextureHandle(_image, _mode);
		switch (_bar_mode)
		{
		case Guage::HP:
			GetHp<T>();
			break;
		case Guage::SPECIAL_ATTACK:
			GetSpecialAttack<T>();
			break;
		default:
			break;
		}
		chara = obj->GetParent()->Component()->GetComponent<T>();
	}

	void WorldToScreenMode(bool _mode,VECTOR3 _plusPos);

	void SetAddMode(bool _mode) {guage->SetAddMode(_mode);}

private:
	float* barValue;
	float barValueMax;
	MeshRenderer2D* guage;
	MeshRenderer2D* edge;
	float displayHp;
	float changeColorCounter;
	bool screenMode;
	VECTOR3 plus;
	bool addMode;
	CharaBase* chara;
};