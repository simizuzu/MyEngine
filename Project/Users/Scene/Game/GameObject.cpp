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
	
	player_ = std::make_unique<Player>();
	player_->Initialize(camera_);

	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	groundModel_.reset(ObjModel::LoadFromObj("ground"));

	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel(skydomeModel_.get());
	groundObj_->SetModel(groundModel_.get());

	skydomeTrans.Initialize();
	groundTrans.Initialize();
	skydomeTrans.SetScale({ 900.0f,900.0f,900.0f });
}

void GameObject::Update()
{
	skydomeTrans.Update(camera_);
	skydomeTrans.SetTranslation({camera_->GetEye()});
	groundTrans.Update(camera_);

	player_->Update();
}

void GameObject::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	groundObj_->Draw(&groundTrans);
	player_->Draw();
	player_->UIDraw();
}
