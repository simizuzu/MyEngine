#include "SceneData.h"

/**
 * @file SceneData.cpp
 * @brief GameSceneのリソースの読み込みについて書かれてあります
 */

void SceneData::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;
	curveData2 = LevelLoader::LoadFile("curveData");
	
	player_ = std::make_unique<Player>();
	player_->Initialize(camera_);

	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	groundModel_.reset(ObjModel::LoadFromObj("ground"));
	//buildingModel_.reset(ObjModel::LoadFromObj("building"));

	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());
	//buildingObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel(skydomeModel_.get());
	//buildingObj_->SetModel(buildingModel_.get());
	groundObj_->SetModel(groundModel_.get());

	skydomeTrans.Initialize();
	//buildingTrans.Initialize();
	groundTrans.Initialize();
	//
	skydomeTrans.SetScale({ 900.0f,900.0f,900.0f });
}

void SceneData::Update()
{
	//groundTrans.SetRotation({ 0,90.0f * MyMathUtility::degree2Radius,0 });
	skydomeTrans.Update(camera_);
	skydomeTrans.SetTranslation({camera_->GetEye()});
	//buildingTrans.Update(camera_);
	groundTrans.Update(camera_);

	player_->Update();
}

void SceneData::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	//buildingObj_->Draw(&buildingTrans);
	groundObj_->Draw(&groundTrans);
	player_->Draw();
	player_->UIDraw();
}
