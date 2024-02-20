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

	//bulletObj.reset(ObjObject3d::Create());
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
	////デスフラグの立った弾を削除
	//bullets.remove_if([](PlayerBullet* bullet) {
	//	if (bullet->IsDead())
	//	{
	//		delete bullet;
	//		return true;
	//	}
	//	return false;
	//	});

	RotateCamera();

	playerTrans.SetRotation({ -10.0f * MyMathUtility::degree2Radius,-20.0f * MyMathUtility::degree2Radius,0});

	//カメラの角度を取得する
	cameraHAngle = camera_->GetHAngle(camera_->GetEye(),camera_->GetTarget()); //水平方向
	cameraVAngle = camera_->GetVAngle(camera_->GetEye(),camera_->GetTarget()); //垂直方向

#ifdef _DEBUG
	ImGui::Begin("StickDeadZone");
	ImGui::Text("DeadZone(%f,%f)", stickDeadZone.x,stickDeadZone.y);
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
	//Attack();

	//弾更新
	/*for (PlayerBullet* bullet : bullets) {
		bullet->Update(camera_.get());
	}*/
	playerTrans.Update(camera_.get());
}

void Player::Draw()
{
	//弾描画
	/*for (PlayerBullet* bullet : bullets) {
		bullet->Draw();
	}*/

	playerObj->Draw(&playerTrans);
}

void Player::RotateCamera()
{
	stickDeadZone = input->GetLeftStickVec();
	//rotationSpeed *= stickDeadZone.y;
	//rotationSpeed *= stickDeadZone.x;
	
	//カメラの回転制御
	if ( input->PushKey(DIK_RIGHT) || input->InputStick(L_RIGHT))
	{
		rot.y += rotationSpeed * stickDeadZone.x;
	}
	if ( input->PushKey(DIK_LEFT) || input->InputStick(L_LEFT) )
	{
		rot.y += rotationSpeed * stickDeadZone.x;
	}

	//回転が[180°,-180°]以内に制御する
	if ( rot.x > -maxValueRotate && rot.x < maxValueRotate )
	{
		if ( input->PushKey(DIK_DOWN) || input->InputStick(L_DOWN) )
		{
			rot.x += rotationSpeed * stickDeadZone.y;
			if ( rot.x > maxRotate )
			{
				rot.x = maxRotate;
			}
		}
		if ( input->PushKey(DIK_UP) || input->InputStick(L_UP) )
		{
			rot.x += rotationSpeed * stickDeadZone.y;
			if ( rot.x < -maxRotate )
			{
				rot.x = -maxRotate;
			}
		}
	}

	camera_->SetRotation({ rot.x,rot.y,zero });
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
