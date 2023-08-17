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

	buildingModel_.reset(ObjModel::LoadFromObj("building"));
	buildingObj_.reset(ObjObject3d::Create());
	buildingObj_->SetModel(buildingModel_.get());

	groundModel_.reset(ObjModel::LoadFromObj("ground"));
	groundObj_.reset(ObjObject3d::Create());
	groundObj_->SetModel(groundModel_.get());

	skydomeTrans.Initialize();
	buildingTrans.Initialize();
	groundTrans.Initialize();
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
	//}]

	//経過時間(elapsedTime[s])の計算
	nowCount++;
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 60.f;

	timeRate = elapsedTime / maxTime;

	camera->Update();
	light->Update();

	skydomeTrans.Update(camera.get());
	buildingTrans.Update(camera.get());
	groundTrans.Update(camera.get());

	fbxObj_->Update(camera.get());
	fbxObj_->SetTranslation({ { MyMathUtility::BezierCurve(levelData->curves, timeRate) } });

	//camera->SetTarget({ 0.0f,100.0f,0.0f });
	
	cameraTrans.translation_ = MyMathUtility::BezierCurve(levelData->curves, timeRate);
	cameraTrans.MakeMatWorld();
	camera->SetEye({ MyMath::GetWorldPosition(cameraTrans) });

	MyMath::Vector3 forward(0.0f, 0.0f, 1.0f);
	forward = MyMath::Vec3Mat4Mul(forward, cameraTrans.matWorld);
	camera->target_ = camera->eye_ + forward;

	MyMath::Vector3 up(0, 1, 0);
	camera->up_ = MyMath::Vec3Mat4Mul(up, cameraTrans.matWorld);
}

void GameScene::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
	buildingObj_->Draw(&buildingTrans);
	groundObj_->Draw(&groundTrans);
	//fbxObj_->Draw();
}

void GameScene::Finalize()
{
}
