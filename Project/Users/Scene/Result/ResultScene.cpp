#include "ResultScene.h"
#include "Numbers.h"

/**
 * @file ResultScene.cpp
 * @brief ResultSceneの処理が書かれてあります
 */

void ResultScene::Initialize()
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

	texClear = TextureManager::Load("Resources/Texture/Scene/GameClear.png");
	spriteClear = std::make_unique<Sprite>();
	spriteClear->Initialize();

	sceneManager_ = SceneManager::GetInstance();
}

void ResultScene::Update()
{
	transition_->DrawWhiteOut();

	if ( input_->MouseButtonTrigger(LEFT) )
	{
		blackoutTimer--;
	}
	if ( blackoutTimer < 59 )
	{
		blackoutTimer--;
		if ( blackoutTimer < 2 )
		{
			sceneManager_->ChangeScene("TITLE");
		}
	}

	camera->Update();
	light->Update();
}

void ResultScene::Draw()
{
//タイトルシーン内の各モデルの描画
	spriteClear->Draw(texClear,{0,0});

	if ( blackoutTimer < 59 )
	{
		transition_->DrawBlackOut();
	}
}

void ResultScene::Finalize()
{
	blackoutTimer = 60;
}
