#include "GameScene.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file GameScene.cpp
 * @brief GameSceneの処理が書かれてあります
 */

void GameScene::Initialize()
{
	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1,1,1 });
	ObjObject3d::SetLight(light.get());

	input_ = Input::GetInstance();

	camera = new Camera();
	camera->Initialize();

	player = new Player();
	player->Initialize(camera);

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize(camera,input_);

	modelData_ = std::make_unique<SceneData>();
	modelData_->Initialize(camera);

	sceneManager_ = SceneManager::GetInstance();
}

void GameScene::Update()
{
	if (input_->TriggerPushKey(DIK_SPACE) || input_->TriggerButton(A))
	{
		sceneManager_->ChangeScene("TITLE");
	}

#ifdef _DEBUG
	ImGui::Begin("CameraPos");
	ImGui::Text("Position(%f,%f,%f)", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
	ImGui::End();
#endif
	camera->Update();
	light->Update();

	modelData_->Update();
	gameCamera_->Update();

	//if (gameCamera_->timeRate >= 1.0f)
	//{
	//	sceneManager_->ChangeScene("TITLE");
	//}
	//player->timeRate = 0.0f;

}

void GameScene::Draw()
{
	modelData_->Draw();
}

void GameScene::Finalize()
{

}
	
	
