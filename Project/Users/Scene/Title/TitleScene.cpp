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
	ObjObject3d::SetLight(light.get());

	titleAnimation_ = std::make_unique<TitleAnimation>();
	titleAnimation_->Initalize(camera.get());

	//モデル
	object.reset(FbxObject3d::Create());
	object->SetModel("KaedeCamera");
	object->PlayAnimation();

	//ワールド座標
	transform.Initialize();
	transform.SetScale({ 0.01f,0.01f ,0.01f });

	keyframeData = LevelLoader::LoadKeyframe("test",animTime);

	sceneManager_ = SceneManager::GetInstance();
}

void TitleScene::Update()
{
	light->Update();

	//進む速度を決める
	animTime += 1.0f;
	animTime = std::fmod(animTime,keyframeData->cameraKeyframe[ "Camera" ].duration);
	//補間された値を座標に入れていく
	translate = MyMathUtility::CalculateValueLerp(keyframeData->cameraKeyframe[ "Camera" ].translate,animTime);
	rotate = MyMathUtility::CalculateValueSlerp(keyframeData->cameraKeyframe[ "Camera" ].rotate,animTime);

	//カメラの座標にセット
	//camera->SetTranslation(translate);
	camera->SetRotation(rotate);
	camera->Update("quaternion");

	transform.SetRotation({0,MyMathUtility::degree2Radius * 180.0f,0.0f});
	transform.Update(camera.get());
	object->Update();

	//タイトルシーン内の各モデルの挙動
	titleAnimation_->Update();
}

void TitleScene::Draw()
{
	//タイトルシーン内の各モデルの描画
	titleAnimation_->Draw();
	object->Draw(&transform);
}

void TitleScene::Finalize()
{
	titleAnimation_->Finalize();
}
