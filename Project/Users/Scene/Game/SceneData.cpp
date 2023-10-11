#include "SceneData.h"

void SceneData::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;
	levelData = LevelLoader::LoadFile("levelData");
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
	skydomeTrans.SetScale({ 500.0f,500.0f,500.0f });
}

void SceneData::Update()
{
	skydomeTrans.Update(camera_);
	//buildingTrans.Update(camera_);
	groundTrans.Update(camera_);

	player_->Update();

	//nowCount++;
	//elapsedCount = nowCount - startCount;
	//float elapsedTime = static_cast<float> (elapsedCount) / 60.0f;
	//timeRate = elapsedTime / maxTime;


	//if (timeRate >= 1.0f)
	//{
	//	if (startIndex < curveData2->curves.size() - 3)
	//	{
	//		startIndex += 1;
	//		timeRate -= 1.0f;
	//		startCount = nowCount;
	//	}
	//	else
	//	{
	//		timeRate = 1.0f;
	//	}
	//}
}

void SceneData::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	//buildingObj_->Draw(&buildingTrans);
	groundObj_->Draw(&groundTrans);
	player_->Draw();
	player_->UIDraw();
}
