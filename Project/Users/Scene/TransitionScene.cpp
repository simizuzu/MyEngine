#include "TransitionScene.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @file TransitionScene.cpp
 * @brief TransitionSceneの処理について書いてあります
 */

TransitionScene* TransitionScene::GetInstance()
{
	static TransitionScene instance;
	return &instance;
}

void TransitionScene::Initialize()
{
	input = Input::GetInstance();

	spriteBlack_ = std::make_unique<Sprite>();
	spriteLineLeft_ = std::make_unique<Sprite>();
	spriteLineRight_ = std::make_unique<Sprite>();
	spriteCircle_ = std::make_unique<Sprite>();
	spriteTriangleLeft_ = std::make_unique<Sprite>();
	spriteTriangleRight_ = std::make_unique<Sprite>();
	spriteWave_ = std::make_unique<Sprite>();

	spriteBlack_->Initialize();
	spriteLineLeft_->Initialize();
	spriteLineRight_->Initialize();
	spriteCircle_->Initialize();
	spriteTriangleLeft_->Initialize();
	spriteTriangleRight_->Initialize();
	spriteWave_->Initialize();

	texBlack_ = TextureManager::Load("Resources/Texture/white1x1.png");
	texLine_ = TextureManager::Load("Resources/Texture/Scene/transition_01.png");
	texCircle_ = TextureManager::Load("Resources/Texture/Scene/title.png");
	texTriangleBack_ = TextureManager::Load("Resources/Texture/Scene/transition_03.png");
	texWave_ = TextureManager::Load("Resources/Texture/Scene/wave.png");

	color_ = { 0.0f,0.0f,0.0f,0.0f };
	lineColor_ = { 1.0f,1.0f,1.0f,1.0f };
	circleColor_ = { 0.3f,1.0f,0.3f,0.3f };
	wavePos_ = { 0.0f,-730.0f };
}

void TransitionScene::DrawBlackOut()
{
	alphaFlag = true;

	if ( alphaFlag )
	{
		color_.w += 0.05f;
		if ( color_.w > static_cast<float>(three) )
		{
			alphaFlag = false;
			color_.w = decimal.zeroPointOne;
		}
	}

	spriteBlack_->SetColor(color_);
	spriteBlack_->Draw(texBlack_,{ zero,zero },windowSize);
}

void TransitionScene::DrawWhiteOut()
{
	if ( color_.w == static_cast< float >( zero) )
	{
		color_.w = static_cast< float >( one );
	}
	else
	{
		color_.w -= texAlpha.black1x1Alpha;
		lineColor_.w += texAlpha.lineAlpha;
		adjustAlpha_ -= texAlpha.adjustAlpha;

		spriteLineLeft_->SetColor(lineColor_);
		spriteLineRight_->SetColor(lineColor_);

		spriteTriangleLeft_->SetColor({ red,green,blue,lineColor_.w - adjustAlpha_ });
		spriteTriangleRight_->SetColor({ red,green,blue,lineColor_.w - adjustAlpha_ });

		spriteBlack_->SetColor(color_);
		spriteBlack_->Draw(texBlack_,texSizeZero,windowSize);
	}

	if ( color_.w < static_cast< float >( zero ) )
	{
		color_.w = static_cast< float >( zero );
		Reset();
	}
}

void TransitionScene::GameOver()
{
	color_.w = static_cast<float>(one);

	wavePos_.y += static_cast< float >( twenty );

	if ( wavePos_.y > static_cast< float >( ten ) )
	{
		wavePos_.y = -initWavePos;
		Reset();
	}

	spriteBlack_->SetColor({ color_ });
	spriteBlack_->Draw(texBlack_,wavePos_,windowSize,texRot,black1x1Size);
	spriteWave_->Draw(texWave_,{ wavePos_.x, wavePos_.y + black1x1PosY });
}

void TransitionScene::Reset()
{
	color_ = texAlpha.initAlphaZero;
	lineColor_ = texAlpha.initAlphaOne;
	texRot = static_cast< float >( zero );
	adjustAlpha_ = static_cast< float >( one );
}

