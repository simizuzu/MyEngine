#include "SceneData.h"

void SceneData::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;
	levelData = LevelLoader::LoadFile("levelData");


	tyoinoriModel_.reset(ObjModel::LoadFromObj("Tyoinori"));
	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	groundModel_.reset(ObjModel::LoadFromObj("ground"));
	buildingModel_.reset(ObjModel::LoadFromObj("building"));

	tyoinoriObj_.reset(ObjObject3d::Create());
	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());
	buildingObj_.reset(ObjObject3d::Create());

	tyoinoriObj_->SetModel(tyoinoriModel_.get());
	skydomeObj_->SetModel(skydomeModel_.get());
	buildingObj_->SetModel(buildingModel_.get());
	groundObj_->SetModel(groundModel_.get());

	tyoinoriTrans.Initialize();
	skydomeTrans.Initialize();
	buildingTrans.Initialize();
	groundTrans.Initialize();
	skydomeTrans.SetScale({ 500.0f,500.0f,500.0f });
}

void SceneData::Update()
{
	tyoinoriTrans.Update(camera_);
	skydomeTrans.Update(camera_);
	buildingTrans.Update(camera_);
	groundTrans.Update(camera_);
}

void SceneData::Draw()
{
	tyoinoriObj_->Draw(&tyoinoriTrans);
	skydomeObj_->Draw(&skydomeTrans);
	buildingObj_->Draw(&buildingTrans);
	groundObj_->Draw(&groundTrans);
}
