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

	clearDirection = ClearScene::GetInstance();
	clearDirection->Initialize(camera);

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
	
	spriteBlack_->SetColor({ red,green,blue,texBlackAlpha });
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
	spriteBlack_->SetColor({ red,green,blue,texBlackAlpha });

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
		blackUpPos.y -= static_cast< float >(zero);
		blackDownPos.y += static_cast< float >( zero );

		StopTimer();

		//シーン移行
		if ( cameraTimeRate >= 1.0f )
		{
			clearDirection->Update();
			//sceneManager_->ChangeScene("TITLE");
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
		spriteStageName01_->Draw(texStageName01_, fieldNameSize);
		robotoObj_->Draw();

		break;
	case GameScene::SCENEFASE::START:
		texBlackAlpha -= decimal.zeroPointOne;
		//transition_->DrawWhiteOut();
		//spriteWhite_->Draw(texWhite_,{ 640,360 },textureSize,0.0f,{ 0.5f,0.5f });
		break;
	case GameScene::SCENEFASE::GAME:
			//シーン移行
		if ( cameraTimeRate >= 1.0f )
		{
			clearDirection->Draw();
			texBlackAlpha += decimal.zeroPointOne / static_cast< float >( two);
			if ( texBlackAlpha > static_cast< float >( one ) )
			{
				sceneManager_->ChangeScene("RESULT");
			}
			
		}
		break;
	default:
		break;
	}

	if ( scene != SCENEFASE::GAME )
	{
		spriteBlackUp_->Draw(texBlackUp_,blackUpPos,{ blackSize.x,blackSize.y });
		spriteBlackDown_->Draw(texBlackDown_,blackDownPos,{ blackSize.x * minus1,blackSize.y });
	}
	spriteBlack_->Draw(texBlackUp_,blackUpPos,windowSize);
}


void GameScene::Finalize()
{

}

void GameScene::StartDirection()
{
	if ( easingFlag == false )
	{
		if ( startCount == three )
		{
			scene = SCENEFASE::GAME;
		}
		else
		{
			texAlpha = static_cast<float>(one);
			easingTimer = static_cast< float >(zero);
			easingFlag = true;
		}
	}

	if ( easingFlag == true )
	{
		easingTimer++;
		decimalAlpha = 0.04f;
		texAlpha -= decimalAlpha;

		textureSize.y = MyMathUtility::EaseOutQuint(static_cast< float >( zero ),static_cast< float >( one ),easingTimer,easingFrame);
		spriteWhite_->SetColor({ COLOR::red,COLOR::green,COLOR::blue,texAlpha });

		/*if ( textureSize.y > 520.0f )
		{
			textureSize.y = 520.0f;
		}*/

		if ( texAlpha < static_cast< float >( zero ) )
		{
			texAlpha = static_cast< float >( zero );
		}

		if ( easingTimer > oneSecondFrame )
		{
			easingTimer = static_cast< float >( zero );
			easingFlag = false;
			texAlpha = static_cast< float >( one );
			startCount++;
		}
	}
}

void GameScene::BlackMind()
{
	camera->SetEye({ -2.2f,1,-4 });
	camera->SetTarget({ -3,2,0 });

	blackSize.x += blackTimer_;

	if ( blackSize.x > stopBlackSize )
	{
		blackSize.x = stopBlackSize;
	}
}

void GameScene::StopTimer()
{
	if ( blackUpPos.y < -stopBlackPos.x )
	{
		blackUpPos.y = -stopBlackPos.x;
		blackDownPos.y = stopBlackPos.y;
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
		texBlackAlpha += decimal.zeroPointOne;
		if ( startCount > 250 )
		{
			texBlackAlpha = static_cast< float >( one );
			startCount = zero;
			scene = SCENEFASE::START;
		}
	}
}

