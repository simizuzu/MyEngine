#include "SceneData.h"

void SceneData::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;
	levelData = LevelLoader::LoadFile("levelData");
	curveData2 = LevelLoader::LoadFile("curveData");

	tyoinoriModel_.reset(ObjModel::LoadFromObj("box"));
	tyoinoriModel2_.reset(ObjModel::LoadFromObj("Tyoinori"));
	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	groundModel_.reset(ObjModel::LoadFromObj("ground"));
	buildingModel_.reset(ObjModel::LoadFromObj("building"));

	tyoinoriObj_.reset(ObjObject3d::Create());
	tyoinoriObj2_.reset(ObjObject3d::Create());
	skydomeObj_.reset(ObjObject3d::Create());
	groundObj_.reset(ObjObject3d::Create());
	buildingObj_.reset(ObjObject3d::Create());

	tyoinoriObj_->SetModel(tyoinoriModel_.get());
	tyoinoriObj2_->SetModel(tyoinoriModel2_.get());
	skydomeObj_->SetModel(skydomeModel_.get());
	buildingObj_->SetModel(buildingModel_.get());
	groundObj_->SetModel(groundModel_.get());

	tyoinoriTrans.Initialize();
	tyoinoriTrans2.Initialize();
	skydomeTrans.Initialize();
	buildingTrans.Initialize();
	groundTrans.Initialize();
	skydomeTrans.SetScale({ 500.0f,500.0f,500.0f });

	for (size_t i = 0; i < objs.size(); i++)
	{
		objs[i].Initialize();
		objs[i].scale_ = { 0.025f,0.025f ,0.025f };
	}
}

void SceneData::Update()
{
	skydomeTrans.Update(camera_);
	buildingTrans.Update(camera_);
	groundTrans.Update(camera_);

	for (size_t i = 0; i < objs.size(); i++)
	{

		float t = 1.0f / objs.size();
		MyMath::Vector3 pos = MyMathUtility::BezierCurve(curveData2->curves, t * i);


		objs[i].translation_ = pos;

		objs[i].Update(camera_);
		
	}

}

void SceneData::Draw()
{
	for (size_t i = 0; i < objs.size(); i++)
	{
		tyoinoriObj_->Draw(&objs[i]);
	}
	
	skydomeObj_->Draw(&skydomeTrans);
	buildingObj_->Draw(&buildingTrans);
	groundObj_->Draw(&groundTrans);
}
