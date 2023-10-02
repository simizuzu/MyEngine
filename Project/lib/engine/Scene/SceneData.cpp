#include "SceneData.h"

void SceneData::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;

	planeModel_.reset(ObjModel::LoadFromObj("mario"));
	planeObj_.reset(ObjObject3d::Create());
	planeObj_->SetModel(planeModel_.get());
	planeTrans.Initialize();

	
}

void SceneData::Update()
{
	rot++;
	//planeTrans.SetRotation({ 0,rot,0 });
	planeTrans.Update(camera_);
}

void SceneData::Draw()
{
	planeObj_->Draw(&planeTrans);
}
