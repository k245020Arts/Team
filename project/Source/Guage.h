#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include "CharaBase.h"
#include "MeshRenderer2D.h"

class Guage : public Component
{
public:
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
		hp = obj->GetParent()->Component()->GetComponent<T>()->GetHp();
		maxhp = obj->GetParent()->Component()->GetComponent<T>()->GetMaxHp();
	}
	/// <summary>
	/// HPƒo[‚Ì˜g‚ğ•`‰æ
	/// </summary>
	/// <param name="_image"></param>
	/// <param name="_mode"></param>
	/// <param name="_transfrom"></param>
	void EdgeDrawReady(int _image, MeshRenderer2D::GraphMode _mode,Transform _transfrom);
	template <typename T>
	void GuageDrawReady(int _image, MeshRenderer2D::GraphMode _mode) {
		guage = obj->Component()->AddComponent<MeshRenderer2D>();
		guage->TextureHandle(_image, _mode);
		GetHp<T>();
	}



private:
	float* hp;
	float maxhp;
	MeshRenderer2D* guage;
	MeshRenderer2D* edge;
	float displayHp;
	float changeColorCounter;

};