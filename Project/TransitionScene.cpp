#include "TransitionScene.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @file TransitionScene.cpp
 * @brief TransitionSceneの処理について書いてあります
 */

	void TransitionScene::Initialize()
{
	input = Input::GetInstance();

	spriteBlack_ = std::make_unique<Sprite>();
	spriteLineLeft_ = std::make_unique<Sprite>();
	spriteLineRight_ = std::make_unique<Sprite>();
	spriteCircle_ = std::make_unique<Sprite>();
	spriteTriangleLeft_ = std::make_unique<Sprite>();
	spriteTriangleRight_ = std::make_unique<Sprite>();

	spriteBlack_->Initialize();
	spriteLineLeft_->Initialize();
	spriteLineRight_->Initialize();
	spriteCircle_->Initialize();
	spriteTriangleLeft_->Initialize();
	spriteTriangleRight_->Initialize();

	texBlack_ = TextureManager::Load("Resources/Texture/white1x1.png");
	texLine_ = TextureManager::Load("Resources/Texture/Scene/transition_01.png");
	texCircle_ = TextureManager::Load("Resources/Texture/Scene/title.png");
	texTriangleBack_ = TextureManager::Load("Resources/Texture/Scene/transition_03.png");

	color_ = { 0.0f,0.0f,0.0f,0.0f };
	lineColor_ = { 1.0f,1.0f,1.0f,1.0f };
	circleColor_ = { 0.3f,1.0f,0.3f,0.3f };
}

void TransitionScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("alpha");
	ImGui::Text("alpha:%f",color_.w);
	ImGui::Text("translation:%f",translation_.x);
	ImGui::End();
#endif // _DEBUG
}

void TransitionScene::Draw()
{
	spriteCircle_->Draw(texCircle_,{ 0,0 });

	switch ( count )
	{
	case TransitionScene::TRANSITION_COUNT::blackOut:
		BlackOut();
		break;
	case TransitionScene::TRANSITION_COUNT::whiteOut:
		WhiteOut();
		break;
	case TransitionScene::TRANSITION_COUNT::Reset:
		Reset();
		break;
	default:
		break;
	}
}

void TransitionScene::Finalize()
{
}

void TransitionScene::BlackOut()
{
	if ( input->MouseButtonTrigger(LEFT) )
	{
		alphaFlag = true;
	}
	if ( alphaFlag )
	{
		color_.w += 0.01f;
		if ( color_.w > 3.0f )
		{
			alphaFlag = false;
			color_.w = 1.0f;
			count = TRANSITION_COUNT::whiteOut;
		}
	}

	spriteBlack_->SetColor({ 0.0f,0.0f,0.0f,color_.w });
	spriteBlack_->Draw(texBlack_,{ 0,0 },{ 1280,720 });
}

void TransitionScene::WhiteOut()
{
	color_.w -= 0.02f;
	lineColor_.w += 0.03f;
	translation_.x += 40.0f;
	adjustAlpha_ -= 0.04f;

	if ( color_.w < 0.0f )
	{
		color_.w = 0.0f;
		count = TRANSITION_COUNT::Reset;
	}

	if ( lineColor_.w > 1.0f )
	{
		lineColor_.w = 1.0f;
	}

	spriteLineLeft_->SetColor({ 1.0f,1.0f,1.0f,lineColor_.w });
	spriteLineRight_->SetColor({ 1.0f,1.0f,1.0f,lineColor_.w });

	spriteTriangleLeft_->SetColor({ 1.0f,1.0f,1.0f,lineColor_.w - adjustAlpha_ });
	spriteTriangleRight_->SetColor({ 1.0f,1.0f,1.0f,lineColor_.w - adjustAlpha_ });

	spriteLineLeft_->Draw(texLine_,{ 600 - translation_.x,0 },{ 1,1 },0,{ 0.5f,0.0f });
	spriteLineRight_->Draw(texLine_,{ 680 + translation_.x,0 },{ 1,1 },0,{ 0.5f,0.0f },true);

	spriteTriangleLeft_->Draw(texTriangleBack_,{ 300 - translation_.x,0 },{ 1,1 },0,{ 0.5f,0.0f });
	spriteTriangleRight_->Draw(texTriangleBack_,{ 980 + translation_.x,0 },{ 1,1 },0,{ 0.5f,0.0f },true);

	spriteBlack_->SetColor({ 0.0f,0.0f,0.0f,color_.w });
	spriteBlack_->Draw(texBlack_,{ 0,0 },{ 1280,720 });
}

void TransitionScene::LineMove()
{

}

void TransitionScene::rotCircle()
{

}

void TransitionScene::Reset()
{
	if ( input->TriggerPushKey(DIK_R) )
	{
		count = TRANSITION_COUNT::Reset;
		color_ = { 0.0f,0.0f,0.0f,0.0f };
		lineColor_ = { 1.0f,1.0f,1.0f,1.0f };
		circleColor_ = { 0.3f,1.0f,0.3f,0.3f };
		translation_.x = 0.0f;
		circleRot = 0.0f;
		adjustAlpha_ = 1.0f;
		count = TRANSITION_COUNT::blackOut;
	}

}
