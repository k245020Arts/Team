#pragma once
#include "UIBase.h"
#include "../Color/Color.h"

class TextRenderer : public UIBase
{
public:
	TextRenderer();
	~TextRenderer();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// 描画するテキストのセッティング
	/// </summary>
	/// <param name="_text">描画するテキスト</param>
	/// <param name="_pos">座標</param>
	/// <param name="_fileName">ファイルの名前</param>
	/// <param name="_rgb">色</param>
	/// <param name="_scale">拡大率</param>
	/// <param name="_size">フォントの基礎の大きさ</param>
	void TextSetting(std::string _text,const VECTOR2F& _pos,std::string _fileName,const Color::Rgb& _rgb,const VECTOR2F& _scale,int _size);

	void SetPosition(VECTOR2F _pos) { position = _pos; }
	void SetScale(VECTOR2F _scale) { scale = _scale; }

private:
	VECTOR2F position;
	int fontHandle;
	std::string text;
	Color::Rgb rgb;
	VECTOR2F scale;
};
