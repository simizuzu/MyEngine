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

	input = Input::GetInstance();
	camera_.reset(camera);

	bulletObj.reset(ObjObject3d::Create());
	playerObj.reset(ObjObject3d::Create());
	playerObj->SetModel("gun",true);

	//プレイヤーのトランスフォーム初期化
	playerTrans.Initialize();
	//3Dレティクルのトランスフォーム初期化
	worldTransform3DReticle.Initialize();

	//親子関係
	playerTrans.parentMat = &camera_->matCameraWorld_;

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributePlayer);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributePlayer);
}

void Player::Update()
{
	//デスフラグの立った弾を削除
	bullets.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead())
		{
			delete bullet;
			return true;
		}
		return false;
		});

	RotateCamera();

	playerTrans.SetRotation({ -10.0f * MyMathUtility::degree2Radius,-20.0f * MyMathUtility::degree2Radius,0});

	//カメラの角度を取得する
	cameraHAngle = camera_->GetHAngle(camera_->GetEye(),camera_->GetTarget()); //水平方向
	cameraVAngle = camera_->GetVAngle(camera_->GetEye(),camera_->GetTarget()); //垂直方向

#ifdef _DEBUG
	ImGui::Begin("MousePos");
	ImGui::Text("Position(%f,%f)", mousePos.x, mousePos.y);
	ImGui::End();

	ImGui::Begin("PlayerPos");
	ImGui::Text("CameraEye(%f,%f,%f)",playerTrans.GetTranslation().x,playerTrans.GetTranslation().y,playerTrans.GetTranslation().z);
	ImGui::Text("worldTransform3DReticle(%f,%f,%f)",worldTransform3DReticle.GetTranslation().x,worldTransform3DReticle.GetTranslation().y,worldTransform3DReticle.GetTranslation().z);
	ImGui::End();

	ImGui::Begin("CameraAngle");
	ImGui::Text("CameraAngle(%f,%f)",camera_->GetRotation().x,camera_->GetRotation().y);
	ImGui::End();
#endif

	//float hairetu[3 ];
	//hairetu[ 0 ] = guntrans.x;
	//hairetu[ 1 ] = guntrans.y;
	//hairetu[ 2 ] = guntrans.z;

	//ImGui::Begin("GunTranslation");
	//ImGui::SliderFloat3("Translation",hairetu,-5.0f,5.0f);
	//ImGui::End();

	//guntrans.x = hairetu[ 0 ];
	//guntrans.y = hairetu[ 1 ];
	//guntrans.z = hairetu[ 2 ];

	playerTrans.SetTranslation({guntrans.x,guntrans.y,guntrans.z });
	//攻撃処理
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets) {
		bullet->Update(camera_.get());
	}
	playerTrans.Update(camera_.get());
}

void Player::Draw()
{
	//弾描画
	for (PlayerBullet* bullet : bullets) {
		bullet->Draw();
	}

	playerObj->Draw(&playerTrans);
}

void Player::RotateCamera()
{
	//カメラの回転制御
	if ( input->PushKey(DIK_RIGHT) || input->InputStick(R_RIGHT))
	{
		rot.y += rotationSpeed;
		camera_->SetRotation({rot.x,rot.y,zero});
	}
	if ( input->PushKey(DIK_LEFT) || input->InputStick(R_LEFT) )
	{
		rot.y -= rotationSpeed;
		camera_->SetRotation({ rot.x,rot.y,zero });
	}

	//回転が[180°,-180°]以内に制御する
	if ( rot.x > -maxValueRotate && rot.x < maxValueRotate )
	{
		if ( input->PushKey(DIK_DOWN) || input->InputStick(R_DOWN) )
		{
			rot.x += rotationSpeed;
			camera_->SetRotation({ rot.x,rot.y,zero });
			if ( rot.x > maxRotate )
			{
				rot.x = maxRotate;
			}
		}
		if ( input->PushKey(DIK_UP) || input->InputStick(R_UP) )
		{
			rot.x -= rotationSpeed;
			camera_->SetRotation({ rot.x,rot.y,zero });
			if ( rot.x < -maxRotate )
			{
				rot.x = -maxRotate;
			}
		}
	}
}

void Player::OnCollision()
{
}

void Player::SetParent(const WorldTransform* parent)
{
	playerTrans.parent = parent;
}

const std::list<PlayerBullet*>& Player::GetBullets() const
{
	return bullets;
}

MyMath::Vector3 Player::GetCenterPosition() const
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(playerTrans);
	return worldPos;
}

MyMath::Vector3 Player::GetPlayerWorldPosition()
{
	return MyMath::GetWorldPosition(playerTrans);
}

void Player::Attack()
{
	//弾の速度
	const float bulletSpeed = 8.0f;
	MyMath::Vector3 velocity(0,0,bulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = MyMath::Vec3Mat4Mul(velocity,camera_->matCameraWorld_);
	
	//スペースキーまたはRトリガーを押したとき弾を発射
	if (input->PushKey(DIK_SPACE) || input->PushButton(RT)) {
		bulletIntervalFlag = true;
	}

	if ( bulletIntervalFlag )
	{
		bulletIntervalTimer--;
	}

	if ( bulletIntervalTimer == zero )
	{
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletObj.get(),camera_->GetTranslation(),velocity);

		//弾を登録する
		bullets.push_back(newBullet);

		bulletIntervalFlag = false;
		bulletIntervalTimer = 6;
	}
}

Player::~Player()
{
	for (PlayerBullet* bullet : bullets) {
		delete bullet;
	}
}
