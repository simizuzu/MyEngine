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

	spriteWhite_ = std::make_unique<Sprite>();
	spriteBlackUp_ = std::make_unique<Sprite>();
	spriteBlackDown_ = std::make_unique<Sprite>();

	spriteWhite_->Initialize();
	spriteBlackUp_->Initialize();
	spriteBlackDown_->Initialize();

	texWhite_ = TextureManager::Load("Resources/Texture/white1x1.png");
	texBlackUp_ = TextureManager::Load("Resources/Texture/black1x1.png");
	texBlackDown_ = TextureManager::Load("Resources/Texture/black1x1.png");

	spriteBlackUp_->SetSize({ 1280,100 });
	spriteBlackDown_->SetSize({ 1280,100 });

	sceneManager_ = SceneManager::GetInstance();
}

void GameScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("CameraPos");
	ImGui::Text("Position(%f,%f,%f)",camera->GetTarget().x,camera->GetTarget().y,camera->GetTarget().z);
	ImGui::Text("size(%f,%f)",blackUpPos.y,blackDownPos.y);
	ImGui::End();
#endif
	camera->Update();
	light->Update();

	//カメラの挙動
	gameCamera_->Update();

	modelData_->Update();

	switch ( scene )
	{
	case GameScene::SCENEFASE::BLACKMIND:

		//スタート演出
		BlackMind();
		break;
	case GameScene::SCENEFASE::START:

		//スタート演出
		StartDirection();

		break;
	case GameScene::SCENEFASE::GAME:
		blackUpPos.y -= 10.0f;
		blackDownPos.y += 10.0f;

		StopTimer();

		//シーン移行
		if ( input_->TriggerPushKey(DIK_SPACE) || input_->TriggerButton(A) )
		{
			sceneManager_->ChangeScene("TITLE");
		}
		break;
	default:
		break;
	}
}

void GameScene::Draw()
{
	modelData_->Draw();
	spriteWhite_->Draw(texWhite_,{ 640,360 },textureSize,0.0f,{ 0.5f,0.5f });

	spriteBlackUp_->Draw(texBlackUp_,blackUpPos,{ blackSize.x,blackSize.y });
	spriteBlackDown_->Draw(texBlackDown_,blackDownPos,{ blackSize.x * minus1,blackSize.y });

}

void GameScene::Finalize()
{

}

void GameScene::StartDirection()
{
	if ( easingFlag == false )
	{
		if ( startCount == 3 )
		{
			scene = SCENEFASE::GAME;
		}
		else
		{
			easingTimer = 0.0f;
			easingFlag = true;
		}
	}
	if ( easingFlag == true )
	{
		easingTimer++;
		texAlpha -= 0.04f;

		textureSize.y = MyMathUtility::EaseOutQuint(0.0f,1.0f,easingTimer,easingFrame);
		spriteWhite_->SetColor({ COLOR::red,COLOR::green,COLOR::blue,texAlpha });

		if ( textureSize.y > 520.0f )
		{
			textureSize.y = 520.0f;
		}

		if ( texAlpha < 0.0f )
		{
			texAlpha = 0.0f;
		}

		if ( easingTimer > 60.0f )
		{
			easingTimer = 0.0f;
			easingFlag = false;
			texAlpha = 1.0f;
			startCount++;
		}
	}
}

void GameScene::BlackMind()
{
	blackSize.x += blackTimer_;

	if ( blackSize.x > 1300.0f )
	{
		blackSize.x = 1300.0f;
		scene = SCENEFASE::START;
	}
}

void GameScene::StopTimer()
{
	if ( blackUpPos.y < -200.0f )
	{
		blackUpPos.y = -200.0f;
		blackDownPos.y = 800.0f;
	}
}

