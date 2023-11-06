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

	transition_ = TransitionScene::GetInstance();
	transition_->Initialize();

	sceneManager_ = SceneManager::GetInstance();
}

void TitleScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("blackoutTimer");
	ImGui::Text("blackoutTimer:%d",blackoutTimer);
	ImGui::End();
#endif

	if ( input_->MouseButtonTrigger(LEFT)) 
	{
		blackoutTimer--;
	}
	if ( blackoutTimer < 59 )
	{
		blackoutTimer--;
		if ( blackoutTimer < 2 )
		{
			sceneManager_->ChangeScene("GAME");
		}
	}

	camera->Update();
	light->Update();

	//タイトルシーン内の各モデルの挙動
	titleAnimation_->Update();
}

void TitleScene::Draw()
{
	//タイトルシーン内の各モデルの描画
	titleAnimation_->Draw();

	if ( blackoutTimer < 59 )
	{
		transition_->DrawBlackOut();
	}

	if ( flag == true )
	{
		transition_->GameOver();
		blackoutTimer--;
		if ( blackoutTimer < 24 )
		{
			flag = false;
			blackoutTimer = (uint8_t)oneSecondFrame;
			sceneManager_->ChangeScene("TITLE");
		}
	}
}

void TitleScene::Finalize()
{
	blackoutTimer = 60;
}
