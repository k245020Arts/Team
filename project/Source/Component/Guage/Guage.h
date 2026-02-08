#pragma once
#include "../Component.h"
#include "../ComponentManager.h"
#include "../../CharaBase/CharaBase.h"
#include "../MeshRenderer2D/MeshRenderer2D.h"
#include "../UI/UIBase.h"

class Guage : public UIBase
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
	/// HPゲージのHpを取得
	/// </summary>
	/// <typeparam name="T">誰のHPを取得したいか</typeparam>
	template <typename T>
	void GetHp() {
		barValue = obj->GetParent()->Component()->GetComponent<T>()->GetHp();
		barValueMax = obj->GetParent()->Component()->GetComponent<T>()->GetMaxHp();
	}
	/// <summary>
	/// 必殺技ゲージの取得
	/// </summary>
	/// <typeparam name="T">誰の必殺技ゲージか</typeparam>
	template <typename T>
	void GetSpecialAttack() {
		barValue = obj->GetParent()->Component()->GetComponent<T>()->GetSpecialAttackGuage();
		barValueMax = obj->GetParent()->Component()->GetComponent<T>()->GetMaxSpecialAttackGuage();
	}

	/// <summary>
	/// HPバーの枠を描画
	/// </summary>
	/// <param name="_image"></param>
	/// <param name="_mode"></param>
	/// <param name="_transfrom"></param>
	void EdgeDrawReady(int _image, MeshRenderer2D::GraphMode _mode,Transform _transfrom);
	/// <summary>
	/// ゲージを描画する前に設定する
	/// </summary>
	/// <typeparam name="T">誰のゲージを生成したいか</typeparam>
	/// <param name="_image">画像ハンドル</param>
	/// <param name="_mode">描画方法</param>
	/// <param name="_bar_mode">どのモードか</param>
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
	
	/// <summary>
	/// ワールド座標をスクリーンモードで描画をするかを設定
	/// </summary>
	/// <param name="_mode">ワールド座標をスクリーンモードならtrue</param>
	/// <param name="_plusPos">位置の調整</param>
	void WorldToScreenMode(bool _mode,VECTOR3 _plusPos);

	/// <summary>
	/// 加算合成モードかどうか
	/// </summary>
	/// <param name="_mode">加算合成モードならtrue</param>
	void SetAddMode(bool _mode) const  {guage->SetAddMode(_mode);}

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