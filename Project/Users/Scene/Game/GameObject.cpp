#include "GameObject.h"

/**
 * @file GameObject.cpp
 * @brief GameSceneのリソースの読み込みについて書かれてあります
 */

void GameObject::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;
	curveData2 = LevelLoader::LoadFile("curveData");

	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	groundModel_.reset(ObjModel::LoadFromObj("ground"));

	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel(skydomeModel_.get());
	groundObj_->SetModel(groundModel_.get());

	display = std::make_unique<Sprite>();
	reticle = std::make_unique<Sprite>();
	display->Initialize();
	reticle->Initialize();

	displayTex = TextureManager::Load("Resources/Texture/display.png");
	reticleTex = TextureManager::Load("Resources/Texture/reticle.png");

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
}
