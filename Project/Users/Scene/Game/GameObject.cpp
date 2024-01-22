#include "GameObject.h"

/**
 * @file GameObject.cpp
 * @brief GameSceneのリソースの読み込みについて書かれてあります
 */

void GameObject::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	groundModel_.reset(ObjModel::LoadFromObj("ground"));

	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel(skydomeModel_.get());
	groundObj_->SetModel(groundModel_.get());

	display = std::make_unique<Sprite>();
	reticle = std::make_unique<Sprite>();
	UI01 = std::make_unique<Sprite>();
	UI02 = std::make_unique<Sprite>();
	UIRT = std::make_unique<Sprite>();
	UIR = std::make_unique<Sprite>();
	display->Initialize();
	reticle->Initialize();
	UI01->Initialize();
	UI02->Initialize();
	UIRT->Initialize();
	UIR->Initialize();

	displayTex = TextureManager::Load("Resources/Texture/display.png");
	reticleTex = TextureManager::Load("Resources/Texture/reticle.png");
	UI01Tex = TextureManager::Load("Resources/Texture/UI.png");
	UIRTTex = TextureManager::Load("Resources/Texture/RT.png");
	UIRTex = TextureManager::Load("Resources/Texture/R.png");

	skydomeTrans.Initialize();
	groundTrans.Initialize();
	skydomeTrans.SetScale({ 900.0f,900.0f,900.0f });
}

void GameObject::Update()
{
	skydomeTrans.Update(camera_);
	skydomeTrans.SetTranslation({camera_->GetTranslation()});
	groundTrans.SetRotation({0.0f,180.0f*MyMathUtility::degree2Radius,0.0f });
	groundTrans.Update(camera_);
}

void GameObject::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	groundObj_->Draw(&groundTrans);
}

void GameObject::TexDraw()
{
	display->Draw(displayTex,{ 0,0 });
	reticle->Draw(reticleTex,{ 640,360 },{ 1.5f,1.5f },0,{ 0.5f,0.5f });
	UI01->Draw(UI01Tex,{ 1100,600 });
	UI02->Draw(UI01Tex,{ 980,600 });
	UIRT->Draw(UIRTTex,{ 1015,615 },{ 0.6f,0.6f });
	UIR->Draw(UIRTex,{ 1135,615 },{0.6f,0.6f});
}
