#include "GameScene.h"

#include <imgui.h>

void GameScene::Initialize()
{
	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1,1,1 });
	ObjObject3d::SetLight(light.get());

	input_ = Input::GetInstance();

	camera = std::make_unique<Camera>();
	camera->Initialize();

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize(camera.get());

	modelData_ = std::make_unique<SceneData>();
	modelData_->Initialize(camera.get());


	sceneManager_ = SceneManager::GetInstance();
}

void GameScene::Update()
{
	//if (input_->TriggerPushKey(DIK_SPACE) || input_->TriggerButton(A))
	//{
	//	sceneManager_->ChangeScene("TITLE");
	//}]

	camera->Update();
	light->Update();

	gameCamera_->Update();
	modelData_->Update();
}

void GameScene::Draw()
{
	modelData_->Draw();
}

void GameScene::Finalize()
{
}
