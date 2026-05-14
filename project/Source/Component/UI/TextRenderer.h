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
	void TextSetting(std::string _text, std::string _fileName, std::string _exten, const Color::Rgb& _rgb, int _size, Font_ID::Font_ID _id);

	void SetColor(Color::Rgb _rgba) { rgb = _rgba; }
	void SetAlpha(float _alpha) { rgb.a = _alpha; }

	void SetText(std::string _text) { text = _text; }

	int GetTextWidth();

private:
	int fontHandle;
	std::string text;
	Color::Rgb rgb;
};
