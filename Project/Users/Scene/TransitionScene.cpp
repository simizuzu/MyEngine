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
		if ( color_.w > 3.0f )
		{
			alphaFlag = false;
			color_.w = 1.0f;
		}
	}

	spriteBlack_->SetColor({ 0.0f,0.0f,0.0f,color_.w });
	spriteBlack_->Draw(texBlack_,{ 0,0 },windowSize);
}

void TransitionScene::DrawWhiteOut()
{
	if ( color_.w == 0.0f )
	{
		color_.w = 1.0f;
	}
	else
	{
		color_.w -= 0.09f;
		lineColor_.w += 0.03f;
		translation_.x += 40.0f;
		adjustAlpha_ -= 0.04f;

		//if ( lineColor_.w > 1.0f )
		//{
		//	lineColor_.w = 1.0f;
		//}

		spriteLineLeft_->SetColor({ red,green,blue,lineColor_.w });
		spriteLineRight_->SetColor({ red,green,blue,lineColor_.w });

		spriteTriangleLeft_->SetColor({ red,green,blue,lineColor_.w - adjustAlpha_ });
		spriteTriangleRight_->SetColor({ red,green,blue,lineColor_.w - adjustAlpha_ });

		spriteLineLeft_->Draw(texLine_,{ 600 - translation_.x,0 },{ 1,1 },0,{ 0.5f,0.0f });
		spriteLineRight_->Draw(texLine_,{ 680 + translation_.x,0 },{ 1,1 },0,{ 0.5f,0.0f },true);

		spriteTriangleLeft_->Draw(texTriangleBack_,{ 300 - translation_.x,0 },{ 1,1 },0,{ 0.5f,0.0f });
		spriteTriangleRight_->Draw(texTriangleBack_,{ 980 + translation_.x,0 },{ 1,1 },0,{ 0.5f,0.0f },true);

		spriteBlack_->SetColor({ 0.0f,0.0f,0.0f,color_.w });
		spriteBlack_->Draw(texBlack_,{ 0,0 },windowSize);
	}

	if ( color_.w < 0.0f )
	{
		color_.w = 0.0f;
		Reset();
	}
}

void TransitionScene::GameOver()
{
	color_.w = 1.0f;

	wavePos_.y += 20.0f;

	if ( wavePos_.y > 10.0f )
	{
		wavePos_.y = -730.0f;
		Reset();
	}

	spriteBlack_->SetColor({ color_ });
	spriteBlack_->Draw(texBlack_,wavePos_,windowSize,0,{0,0.05f});
	spriteWave_->Draw(texWave_,{ wavePos_.x,wavePos_.y + 680.0f });
}

void TransitionScene::Reset()
{
	color_ = { 0.0f,0.0f,0.0f,0.0f };
	lineColor_ = { 1.0f,1.0f,1.0f,1.0f };
	circleColor_ = { 0.3f,1.0f,0.3f,0.3f };
	translation_.x = 0.0f;
	circleRot = 0.0f;
	adjustAlpha_ = 1.0f;
}
