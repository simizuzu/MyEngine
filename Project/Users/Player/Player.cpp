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
	camera_.reset(camera);
	//操作関連の初期化
	input = Input::GetInstance();
	
	//視野角の設定
	camera_->SetFovAngleY(MyMathUtility::degree2Radius * degree90);
	//blenderからカーブデータを取得
	curveData = LevelLoader::LoadFile("curveData");
	//スタート地点を今の地点にセット
	nowCount = startCount;
	//次の地点を算出
	SplinePointLineUp(curveData->curves);

	playerObj.reset(ObjObject3d::Create());
	playerObj->SetModel("gun",true);

	//プレイヤーのトランスフォーム初期化
	playerTrans.Initialize();
	//カメラを親に設定
	playerTrans.parentMat = &camera_->matCameraWorld_;
	playerTrans.SetTranslation({0,100,0});

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributePlayer);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributePlayer);
}

void Player::Update()
{
	//カメラの回転処理
	RotateCamera();

	//レールカメラの移動処理
	RailCamera();

	playerTrans.SetTranslation(gunmodelTranslation);
	playerTrans.SetRotation({ -10.0f * MyMathUtility::degree2Radius,-20.0f * MyMathUtility::degree2Radius,0 });
	playerTrans.Update(camera_.get());

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::Text("CameraEye(%f,%f,%f)",playerTrans.GetTranslation().x,playerTrans.GetTranslation().y,playerTrans.GetTranslation().z);
	ImGui::End();
#endif
}

void Player::Draw()
{
	playerObj->Draw(&playerTrans);
}

void Player::RotateCamera()
{
	//スティックのベクトルを取得
	stickDeadZone = input->GetLeftStickVec();

	//カメラの回転制御
	if ( input->InputStick(L_RIGHT))
	{
		rot.y += rotationSpeed * stickDeadZone.x;
	}
	if (  input->InputStick(L_LEFT) )
	{
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
	
}

void Player::SplinePointLineUp(std::vector<LevelData::CurveData> curvePoint)
{
	points.resize(curvePoint.size() + two);
	for ( size_t i = zero; i < curvePoint.size(); i++ )
	{
		points[ i + one ] = curvePoint[ i ];
		if ( i == zero )
		{
			points[ i ] = curvePoint[ i ];
		}
		if ( i == curvePoint.size() - one )
		{
			points[ i + 2 ] = curvePoint[ i ];
		}
	}
}

void Player::RailCamera()
{
	translation = MyMathUtility::SplinePosition(points,timeRate,startIndex);

	nowCount++;
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast< float > ( elapsedCount ) / oneSecondFrame;
	timeRate = elapsedTime / maxTime;

	if ( timeRate >= static_cast< float >( one ) )
	{
		if ( startIndex < points.size() - three )
		{
			startIndex += static_cast< size_t >( one );
			timeRate -= static_cast< float >( one );
			startCount = nowCount;
		}
		else
		{
			timeRate = static_cast< float >( one );
		}
	}

	camera_->SetTranslation(translation);
}

MyMath::Vector3 Player::GetCenterPosition() const
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(camera_->GetMatWorld());
	return worldPos;
}
