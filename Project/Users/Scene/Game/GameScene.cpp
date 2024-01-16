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

	gpuParticle = std::make_unique<GPUParticle>();
	gpuParticle->Initialize(10000);

	camera = new Camera();
	camera->Initialize();

	transform.Initialize();

	sceneManager_ = SceneManager::GetInstance();
}

void GameScene::Update()
{
	light->Update();
	gpuParticle->Update(0.013f); //1Fの経過時間が役0.013f
	transform.UpdateParticle(camera,true);
}

void GameScene::Draw()
{
	gpuParticle->Draw(&transform);
}

void GameScene::Finalize()
{

}
