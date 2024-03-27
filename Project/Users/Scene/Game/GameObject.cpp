#include "GameObject.h"

/**
 * @file GameObject.cpp
 * @brief GameSceneのリソースの読み込みについて書かれてあります
 */

void GameObject::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;
	input = input->GetInstance();

	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel("skydome",true);
	groundObj_->SetModel("city");

	display = std::make_unique<Sprite>();
	UI01 = std::make_unique<Sprite>();
	UI02 = std::make_unique<Sprite>();
	UIRT = std::make_unique<Sprite>();
	UIL = std::make_unique<Sprite>();
	display->Initialize();
	UI01->Initialize();
	UI02->Initialize();
	UIRT->Initialize();
	UIL->Initialize();

	displayTex = TextureManager::Load("Resources/Texture/display.png");
	UI01Tex = TextureManager::Load("Resources/Texture/UI.png");
	UIRTTex = TextureManager::Load("Resources/Texture/RT.png");
	UILTex = TextureManager::Load("Resources/Texture/L.png");

	skydomeTrans.Initialize();
	groundTrans.Initialize();
	groundTrans.SetTranslation({0,-50.0f,0});
	skydomeTrans.SetScale({ 900.0f,900.0f,900.0f });
}

void GameObject::Update()
{
	skydomeTrans.Update(camera_);
	skydomeTrans.SetTranslation(MyMath::GetWorldPosition(camera_->GetMatWorld()));
	groundTrans.SetRotation({ 0.0f,180.0f * MyMathUtility::degree2Radius,0.0f });
	groundTrans.Update(camera_);
}

void GameObject::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	groundObj_->Draw(&groundTrans);
	if ( input->PushButton(RT) )
	{
		UIRT->SetColor({ 0,1,0,1 });
	}
	else
	{
		UIRT->SetColor({ 1,1,1,1 });
	}

	if ( input->InputStick(L_UP) || input->InputStick(L_DOWN) || input->InputStick(L_LEFT) || input->InputStick(L_RIGHT) )
	{
		UIL->SetColor({ 0,1,0,1 });
	}
	else
	{
		UIL->SetColor({ 1,1,1,1 });
	}
}

void GameObject::TexDraw()
{
	display->Draw(displayTex,{ 0,0 });
	UI01->Draw(UI01Tex,{ 1100,600 });
	UI02->Draw(UI01Tex,{ 980,600 });
	UIRT->Draw(UIRTTex,{ 1015,615 },{ 0.6f,0.6f });
	UIL->Draw(UILTex,{ 1135,615 },{ 0.6f,0.6f });
}
