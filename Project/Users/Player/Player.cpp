#include "Player.h"
#include "Numbers.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file Player.cpp
 * @brief Playerの処理が書かれてあります
 */

void Player::Initialize(Camera* camera)
{
	assert(camera);

	//カメラをセット
	camera_ = camera;
	//操作関連の初期化
	input = Input::GetInstance();
	
	//視野角の設定
	camera_->SetFovAngleY(MyMathUtility::degree2Radius * degree90);
	//blenderからカーブデータを取得
	keyframeData = LevelLoader::LoadKeyframe("camera",animTime);
	//スタート地点を今の地点にセット
	nowCount = startCount;

	playerObj.reset(ObjObject3d::Create());
	playerObj->SetModel("gun",true);

	shuttleObj.reset(ObjObject3d::Create());
	shuttleObj->SetModel("SpaceShuttle");

	//プレイヤーのトランスフォーム初期化
	playerTrans.Initialize();
	//カメラを親に設定
	playerTrans.parentMat = &camera_->matCameraWorld_;
	playerTrans.SetTranslation({0,100,0});

	//プレイヤーのトランスフォーム初期化
	shuttleTrans.Initialize();

	hpUI = std::make_unique<Sprite>();
	hpUI->Initialize();
	texHp = TextureManager::Load("Resources/Texture/HP.png");

	//弾のモデル
	bulletObj.reset(ObjObject3d::Create());
	//バレットマネージャ
	bulletManager_ = BulletManager::GetInstance();
	bulletManager_->Initialize("box",camera_);

	//reticle3d = std::make_unique<Reticle3D>();

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributePlayer);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributePlayer);
}

void Player::Update()
{
	gunmodelTranslation = { 0.3f,-0.15f,1.5f };
	const MyMath::Vector3 scale = {1.5f,1.0f, 1.5f};

	//弾の更新
	bulletManager_->Update();

	//カメラの回転処理
	RotateCamera();

	//レールカメラの移動処理	
	RailCamera();

	//攻撃処理
	Attack();

	//銃のモデルの座標を設定
	playerTrans.SetTranslation(gunmodelTranslation);
	playerTrans.SetRotation({ -10.0f * MyMathUtility::degree2Radius,-20.0f * MyMathUtility::degree2Radius,0 });
	playerTrans.Update(camera_);

	//スペースシャトルを見える位置の下に設定
	shuttleTrans.SetTranslation(GetCenterPosition());
	shuttleTrans.SetScale(scale);
	shuttleTrans.translation_.y += -2.0f;
	MyMath::Vector3 vec = translation - oldTranslation;
	shuttleTrans.LookAtMatrix(vec.Norm(),{ 0,1,0 },camera_);

	{
		//自機から3Dレティクルへの距離
		const float distancePlayerTo3DReticle = 50.0f;
		//自機から3Dレティクルへのオフセット(Z+向き)
		MyMath::Vector3 offset = { 0,0,1.0f };
		//自機のワールド行列の回転を反映
		offset = MyMath::Vec3Mat4Mul(offset,reticleTrans.matWorld);
		//ベクトルの長さを整える
		offset = MyMathUtility::MakeNormalize(offset) * distancePlayerTo3DReticle;
	}
}

void Player::Draw()
{
	//銃のモデル描画
	playerObj->Draw(&playerTrans);

	shuttleObj->Draw(&shuttleTrans);

	hpUI->Draw(texHp,{10,600},hpSize);

	//弾の描画
	bulletManager_->Draw();
}

void Player::RotateCamera()
{
	//スティックのベクトルを取得
	stickDeadZone = input->GetLeftStickVec({0.5f,0.5f});

	rotationSpeed = 0.02f;

	//カメラの回転制御
	if ( input->InputStick(L_RIGHT) )
	{
		if ( input->InputStick(R_LEFT) )
		{
			rotationSpeed = 0.01f;
		}
		rot.y += rotationSpeed * stickDeadZone.x;
	}

	if ( input->InputStick(L_LEFT) )
	{
		if ( input->InputStick(R_RIGHT) )
		{
			rotationSpeed = 0.01f;
		}
		rot.y += rotationSpeed * stickDeadZone.x;
	}

	if ( input->PushKey(DIK_RIGHT) )
	{
		rot.y += rotationSpeed;
	}
	if ( input->PushKey(DIK_LEFT) )
	{
		rot.y -= rotationSpeed;
	}

	//回転が[180°,-180°]以内に制御する
	if ( rot.x > -maxValueRotate && rot.x < maxValueRotate )
	{
		if ( input->PushKey(DIK_DOWN) || input->InputStick(L_DOWN) )
		{
			rot.x += rotationSpeed * stickDeadZone.y;
		}
		if ( input->PushKey(DIK_UP) || input->InputStick(L_UP) )
		{
			rot.x += rotationSpeed * stickDeadZone.y;
		}

		if ( input->PushKey(DIK_DOWN))
		{
			rot.x += rotationSpeed;
		}
		if ( input->PushKey(DIK_UP))
		{
			rot.x -= rotationSpeed;
		}

		if ( rot.x > maxRotate )
		{
			rot.x = maxRotate;
		}
		if ( rot.x < -maxRotate )
		{
			rot.x = -maxRotate;
		}
	}

	//制御した回転を親のカメラにセットする
	camera_->SetRotation({ rot.x,rot.y,zero });
}

void Player::OnCollision()
{
	//HPを減らす
	//HP--;

	//ゼロになったら死亡フラグを立てる
	if ( HP == zero )
	{
		isDead = true;
	}
}

void Player::RailCamera()
{
	//進む速度を決める
	animTime += 1.0f / 40.0f;
	animTime = std::fmod(animTime,keyframeData->cameraKeyframe[ "Camera" ].duration);

	//補間された値を座標に入れていく
	playerTranslate = MyMathUtility::CalculateValueLerp(keyframeData->cameraKeyframe[ "Camera" ].translate,animTime);

	//カメラの座標にセット
	camera_->SetTranslation(playerTranslate);
}

void Player::Attack()
{
	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = MyMath::Vec3Mat4Mul(velocity,camera_->GetMatWorld());

	if ( input->PushKey(DIK_SPACE) )
	{
		bulletIntervalFlag = true;
	}

	if ( bulletIntervalFlag )
	{
		bulletIntervalTimer--;
	}

	if ( bulletIntervalTimer == zero )
	{
		bulletManager_->CreatePlayerBullet(GetCenterPosition(),velocity);

		bulletIntervalFlag = false;
		bulletIntervalTimer = 6;
	}
}

MyMath::Vector3 Player::GetCenterPosition() const
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(camera_->GetMatWorld());
	return worldPos;
}

bool Player::IsDead() const
{
	return isDead;
}
