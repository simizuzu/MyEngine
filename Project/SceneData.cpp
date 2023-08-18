#include "SceneData.h"

void SceneData::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;
	levelData = LevelLoader::LoadFile("levelData");

	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	groundModel_.reset(ObjModel::LoadFromObj("ground"));
	buildingModel_.reset(ObjModel::LoadFromObj("building"));

	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());
	buildingObj_.reset(ObjObject3d::Create());

	skydomeObj_->SetModel(skydomeModel_.get());
	buildingObj_->SetModel(buildingModel_.get());
	groundObj_->SetModel(groundModel_.get());

	skydomeTrans.Initialize();
	buildingTrans.Initialize();
	groundTrans.Initialize();
	skydomeTrans.SetScale({ 500.0f,500.0f,500.0f });
}

void SceneData::Update()
{
	skydomeTrans.Update(camera_);
	buildingTrans.Update(camera_);
	groundTrans.Update(camera_);
}

void SceneData::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	buildingObj_->Draw(&buildingTrans);
	groundObj_->Draw(&groundTrans);
}
