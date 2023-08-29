#include "SceneData.h"

void SceneData::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;
	levelData = LevelLoader::LoadFile("levelData");
	curveData2 = LevelLoader::LoadFile("curveData");

	tyoinoriModel_.reset(ObjModel::LoadFromObj("skydome"));
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
}

void SceneData::Update()
{
	tyoinoriTrans.Update(camera_);
	tyoinoriTrans2.Update(camera_);
	skydomeTrans.Update(camera_);
	buildingTrans.Update(camera_);
	groundTrans.Update(camera_);

	//tyoinoriTrans.SetScale({ 5.0f,5.0f,5.0f });

	//経過時間(elapsedTime[s])の計算
	nowCount++;
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 60.f;
	//経過時間(elapsedTime)/全体の時間(maxTime)を0~1(timeRate)にする
	timeRate = elapsedTime / maxTime;

	targetTimeRate = timeRate + 0.002f;

	//カメラの座標にベジェ曲線の値を入れる
	tyoinoriTrans.translation_ = MyMathUtility::BezierCurve(curveData2->curves, timeRate);
	tyoinoriTrans2.translation_ = MyMathUtility::BezierCurve(curveData2->curves, targetTimeRate);
}

void SceneData::Draw()
{
	//tyoinoriObj_->Draw(&tyoinoriTrans);
	tyoinoriObj2_->Draw(&tyoinoriTrans);
	skydomeObj_->Draw(&skydomeTrans);
	buildingObj_->Draw(&buildingTrans);
	groundObj_->Draw(&groundTrans);
}
