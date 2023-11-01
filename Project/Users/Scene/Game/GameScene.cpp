#include "GameScene.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file GameScene.cpp
 * @brief GameSceneの処理が書かれてあります
 */

void GameScene::GameTimer()
{
	oneSecond--;
	if ( oneSecond == 0 )
	{
		oneSecond = 60;
		gameTimer_++;
	}
}

void GameScene::Initialize()
{
	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1,1,1 });
	ObjObject3d::SetLight(light.get());

	input_ = Input::GetInstance();

	camera = new Camera();
	camera->Initialize();

	transition_ = TransitionScene::GetInstance();
	transition_->Initialize();

	player = new Player();
	player->Initialize(camera);

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize(camera,input_);

	modelData_ = std::make_unique<SceneData>();
	modelData_->Initialize(camera);

	spriteWhite_ = std::make_unique<Sprite>();
	spriteBlack_ = std::make_unique<Sprite>();
	spriteBlackUp_ = std::make_unique<Sprite>();
	spriteBlackDown_ = std::make_unique<Sprite>();
	spriteStageName01_ = std::make_unique<Sprite>();

	spriteWhite_->Initialize();
	spriteBlack_->Initialize();
	spriteBlackUp_->Initialize();
	spriteBlackDown_->Initialize();
	spriteStageName01_->Initialize();

	texWhite_ = TextureManager::Load("Resources/Texture/white1x1.png");
	texBlack_ = TextureManager::Load("Resources/Texture/black1x1.png");
	texBlackUp_ = TextureManager::Load("Resources/Texture/black1x1.png");
	texBlackDown_ = TextureManager::Load("Resources/Texture/black1x1.png");
	texStageName01_ = TextureManager::Load("Resources/Texture/Scene/stagename01.png");

	robotoModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("roboto"));
	robotoObj_.reset(FbxObject3d::Create());
	robotoObj_->SetModel(robotoModel_.get());
	robotoObj_->PlayAnimation();
	robotoObj_->SetScale({ 0.009f,0.009f ,0.009f });

	spriteBlack_->SetColor({ COLOR::red,COLOR::green,COLOR::blue,texBlackAlpha });
	spriteBlackUp_->SetSize({ 1280,100 });
	spriteBlackDown_->SetSize({ 1280,100 });

	sceneManager_ = SceneManager::GetInstance();
}

void GameScene::Update()
{
	GameTimer();

#ifdef _DEBUG
	ImGui::Begin("debug");
	ImGui::Text("Position(%f,%f,%f)",camera->GetTarget().x,camera->GetTarget().y,camera->GetTarget().z);
	ImGui::Text("size(%f,%f)",blackUpPos.y,blackDownPos.y);
	ImGui::End();

	ImGui::Begin("GameTimer");
	ImGui::Text("GameTimer(%d,%d)",gameTimer_,oneSecond);
	ImGui::End();

#endif
	camera->Update();
	light->Update();

	cameraTimeRate = gameCamera_->timeRate;

	//カメラの挙動
	if ( scene == SCENEFASE::START || scene == SCENEFASE::GAME )
	{
		gameCamera_->Update();

	}

	modelData_->Update();
	spriteBlack_->SetColor({ COLOR::red,COLOR::green,COLOR::blue,texBlackAlpha });

	switch ( scene )
	{
	case GameScene::SCENEFASE::MOVIE:
		//スタート演出
		BlackMind();
		//モデルのムービー演出
		ModelMovie();
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
		if ( cameraTimeRate >= 1.0f )
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

	switch ( scene )
	{
	case GameScene::SCENEFASE::MOVIE:
		spriteStageName01_->Draw(texStageName01_,{ 50,300 });
		robotoObj_->Draw();

		break;
	case GameScene::SCENEFASE::START:

		texBlackAlpha -= 0.1f;
		//transition_->DrawWhiteOut();
		//spriteWhite_->Draw(texWhite_,{ 640,360 },textureSize,0.0f,{ 0.5f,0.5f });
		break;
	case GameScene::SCENEFASE::GAME:
		break;
	default:
		break;
	}

	if ( scene != SCENEFASE::GAME )
	{
		spriteBlackUp_->Draw(texBlackUp_,blackUpPos,{ blackSize.x,blackSize.y });
		spriteBlackDown_->Draw(texBlackDown_,blackDownPos,{ blackSize.x * minus1,blackSize.y });
	}
	spriteBlack_->Draw(texBlackUp_,blackUpPos,{ 1280,720 });
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
			texAlpha = 1.0f;
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
	camera->SetEye({ -2.2f,1,-4 });
	camera->SetTarget({ -3,2,0 });

	blackSize.x += blackTimer_;

	if ( blackSize.x > 1300.0f )
	{
		blackSize.x = 1300.0f;
		//scene = SCENEFASE::BLACKMIND;
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

void GameScene::ModelMovie()
{
	startCount++;
	robotoObj_->Update(camera);
	camera->SetEye({ -2.2f,1,-4 });
	camera->SetTarget({ -3,2,0 });

	if ( startCount > 170 )
	{
		texBlackAlpha += 0.1f;
		if ( startCount > 250 )
		{
			texBlackAlpha = 1.0f;
			startCount = 0;
			scene = SCENEFASE::START;
		}
	}
}

