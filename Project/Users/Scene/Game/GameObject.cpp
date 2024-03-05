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

	//skydomeModel_.reset(ObjModel::LoadFromObj("skydome",true));
	//groundModel_.reset(ObjModel::LoadFromObj("ground"));

	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());
	billTestObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel("skydome",true);
	groundObj_->SetModel("ground");
	billTestObj_->SetModel("billbord");

	display = std::make_unique<Sprite>();
	reticle = std::make_unique<Sprite>();
	UI01 = std::make_unique<Sprite>();
	UI02 = std::make_unique<Sprite>();
	UIRT = std::make_unique<Sprite>();
	UIL = std::make_unique<Sprite>();
	display->Initialize();
	reticle->Initialize();
	UI01->Initialize();
	UI02->Initialize();
	UIRT->Initialize();
	UIL->Initialize();

	displayTex = TextureManager::Load("Resources/Texture/display.png");
	reticleTex = TextureManager::Load("Resources/Texture/reticle.png");
	UI01Tex = TextureManager::Load("Resources/Texture/UI.png");
	UIRTTex = TextureManager::Load("Resources/Texture/RT.png");
	UILTex = TextureManager::Load("Resources/Texture/L.png");

	skydomeTrans.Initialize();
	groundTrans.Initialize();
	billTestTrans.Initialize();
	skydomeTrans.SetScale({ 900.0f,900.0f,900.0f });
}

void GameObject::Update()
{
	skydomeTrans.Update(camera_);
	skydomeTrans.SetTranslation({ camera_->GetTranslation() });
	groundTrans.SetRotation({ 0.0f,180.0f * MyMathUtility::degree2Radius,0.0f });
	groundTrans.Update(camera_);
	billTestTrans.SetTranslation({-93.0f,7.0f,-4.0f});
	billTestTrans.SetScale({50,50,50});
	billTestTrans.Update(camera_,true);
}

void GameObject::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	groundObj_->Draw(&groundTrans);
	billTestObj_->Draw(&billTestTrans);
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
	reticle->Draw(reticleTex,{ 640,360 },{ 1.5f,1.5f },0,{ 0.5f,0.5f });
	UI01->Draw(UI01Tex,{ 1100,600 });
	UI02->Draw(UI01Tex,{ 980,600 });
	UIRT->Draw(UIRTTex,{ 1015,615 },{ 0.6f,0.6f });
	UIL->Draw(UILTex,{ 1135,615 },{ 0.6f,0.6f });
}
