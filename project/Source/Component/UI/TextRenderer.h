#pragma once
#include "UIBase.h"

class TextRenderer : public UIBase
{
public:
	TextRenderer();
	~TextRenderer();

	void Update()override;
	void Draw()override;

	void Init();

private:

};
