#include "GameScene.h"

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

	camera = std::make_unique<Camera>();
	camera->Initialize();

	clearDirection = std::make_unique<ClearScene>();
	clearDirection->Initialize(camera.get());

	modelData_ = std::make_unique<GameObject>();
	modelData_->Initialize(camera.get());

	player_ = std::make_unique<Player>();
	player_->Initialize(camera.get());

	enemyData_ = LevelLoader::LoadFile("enemyData");

	//衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();

	spriteWhite_ = std::make_unique<Sprite>();
	spriteBlack_ = std::make_unique<Sprite>();
	spriteBlackUp_ = std::make_unique<Sprite>();
	spriteBlackDown_ = std::make_unique<Sprite>();
	spriteStageName01_ = std::make_unique<Sprite>();
	spriteFlash_ = std::make_unique<Sprite>();
	sprite2DReticle = std::make_unique<Sprite>();

	spriteWhite_->Initialize();
	spriteBlack_->Initialize();
	spriteBlackUp_->Initialize();
	spriteBlackDown_->Initialize();
	spriteStageName01_->Initialize();
	spriteFlash_->Initialize();
	sprite2DReticle->Initialize();

	texWhite_ = TextureManager::Load("Resources/Texture/white1x1.png");
	texBlackUp_ = TextureManager::Load("Resources/Texture/black1x1.png");
	texBlackDown_ = TextureManager::Load("Resources/Texture/black1x1.png");
	texStageName01_ = TextureManager::Load("Resources/Texture/Scene/stagename01.png");
	texFlash_ = TextureManager::Load("Resources/Texture/muzzleFlash.png");
	texReticle_ = TextureManager::Load("Resources/Texture/reticle.png");

	robotoObj_.reset(FbxObject3d::Create());
	robotoObj_->SetModel("roboto");
	robotoObj_->PlayAnimation();

	robotoTrans.Initialize();
	robotoTrans.SetScale({ 0.009f,0.009f ,0.009f });

	spriteBlack_->SetColor({ red,green,blue,texBlackAlpha });
	spriteBlackUp_->SetSize({ 1280,100 });
	spriteBlackDown_->SetSize({ 1280,100 });

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_.reset(EnemyManager::Create(player_.get(),enemyData_,"mob",camera.get()));

	bulletManager_ = BulletManager::GetInstance();

	sceneManager_ = SceneManager::GetInstance();
}

void GameScene::Update()
{
	GameTimer();

#ifdef _DEBUG
	ImGui::Begin("debug");
	ImGui::Text("Position(%f,%f,%f)",player_->GetCenterPosition().x,player_->GetCenterPosition().y,player_->GetCenterPosition().z);
	ImGui::Text("size(%f,%f)",blackUpPos.y,blackDownPos.y);
	ImGui::End();

	ImGui::Begin("GameTimer");
	ImGui::Text("GameTimer(%d,%d)",gameTimer_,oneSecond);
	ImGui::Text("ray(%f,%f,%f)",rayBullet.dir.x,rayBullet.dir.y,rayBullet.dir.z);
	ImGui::End();

#endif

	light->Update();

	//カメラの挙動
	if ( scene == SCENEFASE::START || scene == SCENEFASE::GAME )
	{
		camera->Update(true);
	}
	else
	{
		camera->Update();
	}

	modelData_->Update();
	player_->Update();
	spriteBlack_->SetColor({ red,green,blue,texBlackAlpha });
	const MyMath::Vector4 flashColor = { 1.0f,1.0f,0.7f,1.0f };
	spriteFlash_->SetColor(flashColor);

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
		if ( !init )
		{
			init = true;
			for ( const std::unique_ptr<BaseEnemy>& enemy : enemyManager_->GetEnemys() )
			{
				WorldTransform trans;
				trans.Initialize();
				trans.SetTranslation(enemy->GetCenterPosition());
				trans.SetScale({ 5,5,5 });
			}
		}
		cameraTimeRate = player_->timeRate;
		player_->Update();
		//衝突判定と応答
		CheckAllCollilsions();

		blackUpPos.y -= static_cast< float >( zero );
		blackDownPos.y += static_cast< float >( zero );
		StopTimer();

		//シーン移行
		if ( cameraTimeRate >= 1.0f )
		{
			scene = SCENEFASE::RESULT;
		}
		else if ( player_->IsDead() )
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
	enemyManager_->Update(player_.get(),enemyData_,"mob");
}

void GameScene::Draw()
{
	modelData_->Draw();

	switch ( scene )
	{
	case GameScene::SCENEFASE::MOVIE:
		spriteStageName01_->Draw(texStageName01_,fieldNameSize);
		robotoObj_->Draw(&robotoTrans);
		break;
	case GameScene::SCENEFASE::START:
		texBlackAlpha -= decimal.zeroPointOne;
		break;
	case GameScene::SCENEFASE::GAME:
		player_->Draw();

		//敵の描画
		enemyManager_->Draw();
		modelData_->TexDraw();
		MuzzleFlashRotation();
		sprite2DReticle->Draw(texReticle_,{ 640,360 },{ 1.5f,1.5f },0,{ 0.5f,0.5f });
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
			texAlpha = static_cast< float >( one );
			easingTimer = static_cast< float >( zero );
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
	robotoTrans.Update(camera.get());
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
	//衝突マネージャのリセット
	collisionManager_->Reset();

	//コライダーをリストに登録
	// --------------プレイヤーについて-------------- //
	collisionManager_->AddCollider(player_.get());
	player_->SetRadius(2.0f);


	// --------------敵全てについて-------------- //
	for ( const std::unique_ptr<BaseEnemy>& enemy : enemyManager_->GetEnemys() )
	{
		//半径を指定
		const float colliderRadius = 20000.0f;
		//エリアの中心を各敵の中心にセットし半径を設定
		enemyErea.center = enemy->GetCenterPosition();
		enemyErea.radius = colliderRadius;
		//カメラの判定をプレイヤーと同じ中心にセットし半径を設定
		cameraBody.center = player_->GetCenterPosition();
		cameraBody.radius = 2.0f;

		//当たっていれば
		if ( CollisionManager::CheckSphre2Sphere(enemyErea,cameraBody) )
		{
			//敵が振り向き弾を発射
			enemy->OnCollision();
		}
	}

	MyMath::Vector3 playerpos = player_->GetCenterPosition();

	MyMath::Vector3 vec = playerpos - enemyBody.center;
	MyMath::Vector3 dir = MyMathUtility::MakeNormalize(vec);

	// --------------プレイヤーの攻撃について-------------- //
	if ( input_->PushButton(RT) || input_->PushKey(DIK_SPACE) )
	{
		bulletIntervalFlag = true;
	}
	if ( bulletIntervalFlag )
	{
		bulletIntervalTimer--;
	}
	//押しっぱなしの時、ずっとレイを飛ばし続けることを阻止
	if ( bulletIntervalTimer == zero )
	{
		//レイの始発点をプレイヤーの中心に設定
		rayBullet.start = player_->GetCenterPosition();
		rayBullet.dir = MyMath::Vec3Mat4Mul({ 0,0.0,1.0 },camera->GetMatRot());

		for ( const std::unique_ptr<BaseEnemy>& enemy : enemyManager_->GetEnemys() )
		{
			if ( !enemy->IsDead() )
			{
				//敵の当たり判定の設定
				const float enemyRadius = 15.0f;
				enemyBody.center = enemy->GetCenterPosition();
				enemyBody.radius = enemyRadius;

				if ( CollisionManager::CheckRay2Sphere(rayBullet,enemyBody) )
				{
					//敵のHPを減らす
					enemy->HitBullet();
				}
			}
		}
		//攻撃時のフラグとタイマーをリセット
		bulletIntervalFlag = false;
		bulletIntervalTimer = six;	//6フレーム
	}

	// --------------敵弾について-------------- //
	for ( const std::unique_ptr<BaseBullet>& bullet : bulletManager_->GetNormalBullets() )
	{
		collisionManager_->AddCollider(bullet.get());
		bullet->SetRadius(3.0f);
	}

	//衝突判定と応答
	collisionManager_->CheckAllCollisions();
}

void GameScene::MuzzleFlashRotation()
{
	const float divTimer = 5.5f;
	const MyMath::Vector2 anchorpoint = { 0.45f,0.45f };

	//攻撃ボタンを押したとき
	if ( input_->PushButton(RT) || input_->PushKey(DIK_SPACE) )
	{
		//タイマーとフリップを使いマズルフラッシュを動かす
		if ( muzzleFlashFlag1 )
		{
			spriteFlash_->Draw(texFlash_,flashPos,{ ( float ) bulletIntervalTimer / divTimer,( float ) bulletIntervalTimer / divTimer },zero,anchorpoint,muzzleFlashFlag1,muzzleFlashFlag2);
			muzzleFlashFlag1 = false;
			muzzleFlashFlag2 = true;
		}
		else if ( !muzzleFlashFlag1 )
		{
			spriteFlash_->Draw(texFlash_,flashPos,{ ( float ) bulletIntervalTimer / divTimer,( float ) bulletIntervalTimer / divTimer },zero,anchorpoint,muzzleFlashFlag1,muzzleFlashFlag2);
			muzzleFlashFlag1 = true;
			muzzleFlashFlag2 = false;
		}
	}
}

