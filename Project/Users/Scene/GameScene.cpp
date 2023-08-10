#include "GameScene.h"

#include <imgui.h>

void GameScene::Initialize()
{
	input_ = Input::GetInstance();

	camera = std::make_unique<Camera>();
	camera->Initialize();

	light.reset(Light::Create());
	light->SetLightColor({ 1,1,1 });
	ObjObject3d::SetLight(light.get());

	skydomeModel_.reset(ObjModel::LoadFromObj("skydome", true));
	skydomeObj_.reset(ObjObject3d::Create());
	skydomeObj_->SetModel(skydomeModel_.get());

	skydomeTrans.Initialize();
	skydomeTrans.SetScale({ 500.0f,500.0f,500.0f });

	//levelLoader_ = std::make_unique<LevelLoader>();
	levelData = LevelLoader::LoadFile("levelData");

	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("boneTest"));
	fbxObj_.reset(FbxObject3d::Create());
	fbxObj_->SetModel(fbxModel_.get());
	fbxObj_->PlayAnimation();
	fbxObj_->SetScale({0.05f,0.05f,0.05f});

	//railCamera_->Initailize(camera.get());

	sceneManager_ = SceneManager::GetInstance();
}

void GameScene::Update()
{
	//if (input_->TriggerPushKey(DIK_SPACE) || input_->TriggerButton(A))
	//{
	//	sceneManager_->ChangeScene("TITLE");
	//}
	camera->Update();
	light->Update();
	skydomeTrans.Update(camera.get());
	fbxObj_->Update(camera.get());

	camera->SetTarget({0,20,0});
	camera->SetEye({0,0,-1000});

	if (input_->PushKey(DIK_RIGHT))
	{
		cameraRot.x--;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		cameraRot.x++;
	}

	for (auto& curveData : levelData->curves)
	{
		points_ = curveData.points;
	}

	//MyMathUtility::BezierCurve(curveData.points, 1.0);

	camera->SetTarget({ cameraRot.x,cameraRot.y,cameraRot.z });
}

void GameScene::Draw()
{
	//skydomeObj_->Draw(&skydomeTrans);
	fbxObj_->Draw();
}

void GameScene::Finalize()
{
}
