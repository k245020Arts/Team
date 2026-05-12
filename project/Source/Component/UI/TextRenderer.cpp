#include "TextRenderer.h"

TextRenderer::TextRenderer()
{
	position = VECTOR2F(0.0f,0.0f);
	fontHandle = -1;
	text = "";
	rgb = Color::Rgb();
	scale = VECTOR2F(1.0f,1.0f);
}

TextRenderer::~TextRenderer()
{
	if (fontHandle = -1) {
		DeleteFontToHandle(fontHandle);
	}
}

void TextRenderer::Update()
{

}

void TextRenderer::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)rgb.a);
    DrawExtendFormatStringToHandle((int)position.x,(int)position.y,(double)scale.x,(double)scale.y,GetColor((int)rgb.r,(int)rgb.g,(int)rgb.b),fontHandle,"%s",text.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
}

void TextRenderer::TextSetting(std::string _text, const VECTOR2F& _pos, std::string _fileName, const Color::Rgb& _rgb, const VECTOR2F& _scale, int _size)
{
	text = _text;
	position = _pos;
	std::string pathName = "data/font/";
	fontHandle = LoadFontDataToHandle((pathName + _fileName).c_str(), _size);
	rgb = _rgb;
	scale = _scale;
}
