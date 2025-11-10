#include "BlurScreen.h"
#include "Screen.h"

//const int BlurScreen::SCREEN_NUM = 2;

BlurScreen::BlurScreen()
{
	for (int i = 0; i < SCREEN_NUM; i++) {
		blurScreen[i] = MakeScreen(Screen::WIDTH, Screen::HEIGHT, true);
	}

	Reset();

	defalutAlpha = 150.0f;
	alpha = defalutAlpha;
}

BlurScreen::~BlurScreen()
{
	ScreenFinish();
}

void BlurScreen::Update()
{
	if (!use) {
		return;
	}

	CurrentScreenSet();

	time -= Time::DeltaTimeRate();
	if (time <= 0.0f && !fadeOut) {
		FeedOut(); //フェードアウト始まり
	}

	//フェードアウトの処理
	if (fadeOut) {

		fadeTime -= Time::DeltaTimeRate();

		if (fadeTime >= 0.0f) {
			alpha = (fadeTime / defalutFadeTime) * defalutAlpha;
		}
		else {
			Reset();
			ScreenFinish();
		}
	}

}

void BlurScreen::Draw()
{
	if (!use) {
		return;
	}

	int drawMode = GetDrawMode(); //今の描画モードを保存

	SetUseSetDrawScreenSettingReset(FALSE);

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	int blendMode, param; //今の描画のブレンドのモードとパラメーターを保存
	GetDrawBlendMode(&blendMode, &param);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));

	screenDraw += Time::DeltaTimeRate();
	if (screenDraw > 0.2f) {
		//DrawExtendGraphF(0.0f, 0.0f, Screen::WIDTH, Screen::HEIGHT, blurScreen[1 - currentScreen], false);
	}
	

	SetDrawBlendMode(blendMode, param); 
	
	SetDrawMode(drawMode);

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, blurScreen[currentScreen], false);

	ScreenFlip();

	currentScreen = 1 - currentScreen;

}

void BlurScreen::Play(float _time, float _fadeTime)
{
	Reset();
	SetUseSetDrawScreenSettingReset(FALSE);//SetDrawScreenをしてもカメラが影響ないようにする
	for (int i = 0; i < SCREEN_NUM; i++) {
		if (blurScreen[i] != -1) {
			DeleteGraph(blurScreen[i]);
		}
		blurScreen[i] = MakeScreen(Screen::WIDTH, Screen::HEIGHT, true); //スクリーンの生成
	}

	alpha = defalutAlpha;

	use = true;
	time = _time;
	fadeTime = _fadeTime;
	defalutFadeTime = _fadeTime;
	fadeOut = false;
}


void BlurScreen::ScreenFinish()
{
	for (int i = 0; i < SCREEN_NUM; i++) {
		if (blurScreen[i] != -1) {
			DeleteGraph(blurScreen[i]);
			blurScreen[i] = -1;
		}
	}
	
	SetDrawScreen(DX_SCREEN_BACK);
}

void BlurScreen::FeedOut()
{
	fadeOut = true;
}

void BlurScreen::Reset()
{
	use = false;

	alpha = 0.0f;

	fadeOut = false;

	time = 0.0f;
	fadeTime = 0.0f;

	currentScreen = 0;
	screenDraw = 0.0f;

}

void BlurScreen::CurrentScreenSet()
{
	if (blurScreen[currentScreen] != -1) {
		SetDrawScreen(blurScreen[currentScreen]); //描画先を更新
	}
}
