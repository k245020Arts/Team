#include "TextRenderer.h"
#include "../Transform/Transform.h"

TextRenderer::TextRenderer()
{
	//position = VECTOR2F(0.0f,0.0f);
	fontHandle = -1;
	text = "";
	rgb = Color::Rgb();
	//scale = VECTOR2F(1.0f,1.0f);
}

TextRenderer::~TextRenderer()
{

}

void TextRenderer::Update()
{

}

void TextRenderer::Draw()
{
	VECTOR3 position = obj->GetTransform()->position;
	VECTOR3 scale = obj->GetTransform()->scale;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)rgb.a);
    DrawExtendFormatStringToHandle((int)position.x,(int)position.y,(double)scale.x,(double)scale.y,GetColor((int)rgb.r,(int)rgb.g,(int)rgb.b),fontHandle,"%s",text.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
}


void TextRenderer::TextSetting(std::string _text, std::string _fileName, std::string _exten, const Color::Rgb& _rgb,int _size, Font_ID::Font_ID _id)
{
	text = _text;
	//position = _pos;
	fontHandle = ResourceLoad::LoadFont(_fileName.c_str(), _exten, _id, _size);
	rgb = _rgb;
	//scale = _scale;
}

int TextRenderer::GetTextWidth()
{
	return GetDrawExtendFormatStringWidthToHandle(obj->GetTransform()->scale.x, fontHandle, text.c_str());
}
