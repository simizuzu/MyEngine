#include "TitleScene.h"
#include "Numbers.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file Titlecene.cpp
 * @brief TitleSceneの処理が書かれてあります
 */

void TitleScene::Initialize()
{
	input_ = Input::GetInstance();

	camera = std::make_unique<Camera>();
	camera->Initialize();

	light.reset(Light::Create());
	light->SetLightColor({ 1,1,1 });

	//モデル
	object.reset(FbxObject3d::Create());
	object->SetModel("KaedeCamera");
	object->PlayAnimation();

	//ワールド座標
	transform.Initialize();
	transform.SetScale({ 0.01f,0.01f ,0.01f });

	keyframeData = LevelLoader::LoadKeyframe("kaedeCamera",animTime);
	
	sceneManager_ = SceneManager::GetInstance();
}

void TitleScene::Update()
{
	//進む速度を決める
	animTime += 1.0f;
	animTime = std::fmod(animTime,keyframeData->cameraKeyframe[ "Camera" ].duration);
	//補間された値を座標に入れていく
	translate = MyMathUtility::CalculateValueLerp(keyframeData->cameraKeyframe[ "Camera" ].translate,animTime);
	rotate = MyMathUtility::CalculateValueSlerp(keyframeData->cameraKeyframe[ "Camera" ].rotate,animTime);

	//カメラの座標にセット
	camera->SetTranslation({0,0,-5.0f});
	//camera->SetRotation(rotate);

	camera->Update("quaternion");

	transform.SetRotation(rotate);
	transform.Update(camera.get());
	object->Update();
}

void TitleScene::Draw()
{
	object->Draw(&transform);
}

void TitleScene::Finalize()
{

}
