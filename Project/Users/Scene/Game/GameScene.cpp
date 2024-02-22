#include "GameScene.h"

#include "EnemyBullet.h"
#include "ModelManager.h"

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

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize(camera,input_);

	modelData_ = std::make_unique<GameObject>();
	modelData_->Initialize(camera);

	player_ = std::make_unique<Player>();
	player_->Initialize(camera);

	//衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();

	//colliderModel_.reset(ObjModel::LoadFromObj("collider"));
	//colliderObj_.reset(ObjObject3d::Create());
	//colliderObj_->SetModel(colliderModel_.get());
	//colliderTrans.Initialize();
	//colliderTrans.SetScale({5,5,5});

	spriteWhite_ = std::make_unique<Sprite>();
	spriteBlack_ = std::make_unique<Sprite>();
	spriteBlackUp_ = std::make_unique<Sprite>();
	spriteBlackDown_ = std::make_unique<Sprite>();
	spriteStageName01_ = std::make_unique<Sprite>();
	spriteFlash_ = std::make_unique<Sprite>();

	spriteWhite_->Initialize();
	spriteBlack_->Initialize();
	spriteBlackUp_->Initialize();
	spriteBlackDown_->Initialize();
	spriteStageName01_->Initialize();
	spriteFlash_->Initialize();

	texWhite_ = TextureManager::Load("Resources/Texture/white1x1.png");
	texBlack_ = TextureManager::Load("Resources/Texture/black1x1.png");
	texBlackUp_ = TextureManager::Load("Resources/Texture/black1x1.png");
	texBlackDown_ = TextureManager::Load("Resources/Texture/black1x1.png");
	texStageName01_ = TextureManager::Load("Resources/Texture/Scene/stagename01.png");
	texFlash_ = TextureManager::Load("Resources/Texture/muzzleFlash.png");

	//robotoModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("roboto"));
	robotoObj_.reset(FbxObject3d::Create());
	robotoObj_->SetModel("roboto");
	robotoObj_->PlayAnimation();

	robotoTrans.Initialize();
	robotoTrans.SetScale({ 0.009f,0.009f ,0.009f });
	//robotoObj_->SetScale({ 0.009f,0.009f ,0.009f });
	
	spriteBlack_->SetColor({ red,green,blue,texBlackAlpha });
	spriteBlackUp_->SetSize({ 1280,100 });
	spriteBlackDown_->SetSize({ 1280,100 });

	/*damageModel.reset(ObjModel::LoadFromObj("box"));
	damageParticle = new ParticleManager();
	damageParticle->Initialize(damageModel.get(),camera);*/

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_.reset(EnemyManager::Create("Resources/csv/enemyPop.csv","boneTest",camera));

	sceneManager_ = SceneManager::GetInstance();
}

void GameScene::Update()
{
	GameTimer();

	const float enemyRadius = 2.0f;

	//球の中心点を各敵の原点に設定
	//sphere.center = enemyManager_->GetEnemyCenterPos();
	sphere.radius = enemyRadius;
	//レイの始発点をプレイヤーの中心に設定
	ray.start = player_->GetCenterPosition();
	MyMath::Vector3 vec(0,0,1);
	ray.dir = MyMath::Vec3Mat4Mul(vec,camera->matCameraWorld_);

	colliderTrans.SetTranslation(sphere.center);

	//colliderTrans.Update(camera);

#ifdef _DEBUG
	ImGui::Begin("debug");
	ImGui::Text("Position(%f,%f,%f)",camera->GetTranslation().x,camera->GetTranslation().y,camera->GetTranslation().z);
	ImGui::Text("size(%f,%f)",blackUpPos.y,blackDownPos.y);
	ImGui::End();

	ImGui::Begin("GameTimer");
	ImGui::Text("GameTimer(%d,%d)",gameTimer_,oneSecond);
	ImGui::Text("HIT(%d,%d)",hit,bulletIntervalFlag);
	ImGui::Text("FlashRot(%d)",muzzleFlashFlag1);
	ImGui::End();

#endif

	light->Update();
	
	cameraTimeRate = gameCamera_->timeRate;

	//カメラの挙動
	if ( scene == SCENEFASE::START || scene == SCENEFASE::GAME )
	{
		gameCamera_->Update();
		camera->Update(true);
	}
	else
	{
		camera->Update();
	}

	modelData_->Update();
	player_->Update();
	spriteBlack_->SetColor({ red,green,blue,texBlackAlpha });
	const MyMath::Vector4 flashColor = {1.0f,1.0f,0.7f,1.0f};
	spriteFlash_->SetColor(flashColor);

	switch ( scene )
	{
	case GameScene::SCENEFASE::INIT:
		enemyManager_->EnemyNormalEmit(player_.get());
		if ( enemyManager_->GetReachCommandFlag() )
		{
			scene = SCENEFASE::MOVIE;
		}
		break;

	case GameScene::SCENEFASE::MOVIE:
		//スタート演出
		BlackMind();
		//モデルのムービー演出
		ModelMovie();

		//damageParticle->Add("1",1,60,{ 0.0f,0.0f,0.0f },1,2);
		//damageParticle->Update();

		break;

	case GameScene::SCENEFASE::START:

		//スタート演出
		StartDirection();

		break;
	case GameScene::SCENEFASE::GAME:
		//衝突判定と応答
		CheckAllCollilsions();

		blackUpPos.y -= static_cast< float >(zero);
		blackDownPos.y += static_cast< float >( zero );
		StopTimer();

		//シーン移行
		if ( cameraTimeRate >= 1.0f )
		{
			scene = SCENEFASE::RESULT;
		}
		break;
	case GameScene::SCENEFASE::RESULT:
		clearDirection->Update();
		break;
	default:
		break;
	}
	enemyManager_->Update();
}

void GameScene::Draw()
{
	modelData_->Draw();
	MuzzleFlashRotation();
	player_->Draw();

	//colliderObj_->Draw(&colliderTrans);

	switch ( scene )
	{
	case GameScene::SCENEFASE::INIT:
		break;

	case GameScene::SCENEFASE::MOVIE:
		spriteStageName01_->Draw(texStageName01_, fieldNameSize);
		robotoObj_->Draw(&robotoTrans);
		
		//damageParticle->Draw();
		break;
	case GameScene::SCENEFASE::START:
		texBlackAlpha -= decimal.zeroPointOne;
		//transition_->DrawWhiteOut();
		//spriteWhite_->Draw(texWhite_,{ 640,360 },textureSize,0.0f,{ 0.5f,0.5f });
		break;
	case GameScene::SCENEFASE::GAME:
		modelData_->TexDraw();	
		break;

	case GameScene::SCENEFASE::RESULT:
		clearDirection->Draw();
		texBlackAlpha += decimal.zeroPointOne / static_cast< float >( two );
		if ( texBlackAlpha > static_cast< float >( one ) )
		{
			sceneManager_->ChangeScene("TITLE");
		}
		break;
	default:
		break;
	}

	//敵の描画
	enemyManager_->Draw();

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
	robotoTrans.Update(camera);
	robotoObj_->Update();
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

void GameScene::CheckAllCollilsions()
{
	const float colliderRadius = 20000.0f;

	//衝突マネージャのリセット
	collisionManager_->Reset();

	//コライダーをリストに登録
	//プレイヤーについて
	collisionManager_->AddCollider(player_.get());
	player_->SetRadius(2.0f);

	//敵全てについて
	for ( const std::unique_ptr<BaseEnemy>& enemy : enemyManager_->GetEnemys() )
	{
		collisionManager_->AddCollider(enemy.get());
		enemy->SetRadius(colliderRadius);
	}

	if ( input_->PushButton(RT) || input_->PushKey(DIK_SPACE) )
	{
		bulletIntervalFlag = true;
	}

	if ( bulletIntervalTimer == zero )
	{
		bulletIntervalFlag = false;
		bulletIntervalTimer = 6;
	}

	if ( bulletIntervalFlag )
	{
		//敵全てについて
		for ( const std::unique_ptr<BaseEnemy>& enemy : enemyManager_->GetEnemys() )
		{
			if ( CollisionManager::CheckRay2Sphere(ray,enemy->GetSphereCenter()) )
			{
				enemy->HitBullet();
				hit = true;
			}
		}
		bulletIntervalTimer--;
	}
	else
	{
		hit = false;
	}

	//衝突判定と応答
	collisionManager_->CheckAllCollisions();
}

void GameScene::MuzzleFlashRotation()
{
	if ( input_->PushButton(RT) || input_->PushKey(DIK_SPACE) )
	{
		if ( muzzleFlashFlag1 )
		{
			spriteFlash_->Draw(texFlash_,flashPos,{ (float)bulletIntervalTimer/5.5f,( float ) bulletIntervalTimer / 5.5f },0,{ 0.45f,0.45f },muzzleFlashFlag1,muzzleFlashFlag2);
			muzzleFlashFlag1 = false;
			muzzleFlashFlag2 = true;
		}
		else if ( !muzzleFlashFlag1 )
		{
			spriteFlash_->Draw(texFlash_,flashPos,{ ( float ) bulletIntervalTimer / 5.5f,( float ) bulletIntervalTimer / 5.5f },0,{ 0.45f,0.45f },muzzleFlashFlag1,muzzleFlashFlag2);
			muzzleFlashFlag1 = true;
			muzzleFlashFlag2 = false;
		}
	}
}
