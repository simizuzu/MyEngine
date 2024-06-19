#include "Camera.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <wrl.h>
#include <d3d12.h>
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

#include "DirectXCommon.h"
#include "WinApp.h"

 /**
 * @file Camera.cpp
 * @brief Cameraの処理について書いてあります
 */

void Camera::Initialize()
{
	eye_ = { 0,0,-distance_ };//視点座標
	target_ = { 0,0,0 };//注視点座標
	up_ = { 0,1,0 };//上方向ベクトル

	nearZ_ = 0.1f;
	farZ_ = 6000.0f;

	// アスペクト比を計算(画面横幅/画面縦幅)
	aspect = 
		static_cast<float>(WinApp::GetInstance()->window_width) /
		static_cast<float>(WinApp::GetInstance()->window_height);
	//定数バッファ生成
	CreateConstBuffer();
	//ビュー行列の初期値設定
	UpdateLookAt();
}

void Camera::Update(const std::string& mode)
{
	if ( mode == "default" )
	{
		UpdateLookAt();	//ビュー行列方式(デフォルト)
	}
	else if( mode == "matrix" )
	{
		UpdateMatrix();	//ワールド行列方式
	}
}

void Camera::LookAtMatrix(const MyMath::Vector3& target,const MyMath::Vector3& up)
{
	target_ = translation_ + target;

	//平行移動行列の計算
	//matTrans = MyMathUtility::MakeTranslation(translation_);

	//X軸,Y軸,Z軸,Direction(距離)
	MyMath::Vector3 X,Y,Z,D;
	D = target_ - translation_;
	D = D.Norm();
	Y = up.Norm();
	X = Y.cross(D);
	X = X.Norm();
	Z = X.cross(Y);
	Z = Z.Norm();

	//新しく回転行列を計算する
	matRot.m[ 0 ][ 0 ] = X.x;
	matRot.m[ 0 ][ 1 ] = X.y;
	matRot.m[ 0 ][ 2 ] = X.z;
	matRot.m[ 0 ][ 3 ] = 0.0f;

	matRot.m[ 1 ][ 0 ] = Y.x;
	matRot.m[ 1 ][ 1 ] = Y.y;
	matRot.m[ 1 ][ 2 ] = Y.z;
	matRot.m[ 0 ][ 3 ] = 0.0f;

	matRot.m[ 2 ][ 0 ] = Z.x;
	matRot.m[ 2 ][ 1 ] = Z.y;
	matRot.m[ 2 ][ 2 ] = Z.z;
	matRot.m[ 2 ][ 3 ] = 0.0f;

	matRot.m[ 3 ][ 0 ] = 0.0f;
	matRot.m[ 3 ][ 1 ] = 0.0f;
	matRot.m[ 3 ][ 2 ] = 0.0f;
	matRot.m[ 3 ][ 3 ] = 1.0f;

	//ワールド行列の合成
	matCameraWorld_ = MyMathUtility::MakeIdentity();
	matCameraWorld_ *= matRot;
	//matCameraWorld_ *= matTrans;
}

void Camera::CreateConstBuffer()
{
	HRESULT result;

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataViewProjection) + 0xff) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constBuff_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuff_->Map(0,nullptr,( void** ) &constBuffMap);//マッピング
	assert(SUCCEEDED(result));
}

void Camera::UpdateLookAt()
{
	// ビュー行列の生成
	matView_ = MyMathUtility::MakeLookAtLH(eye_, target_, up_);
	// 逆行列
	matViewInverse_ = MyMathUtility::MakeInverse(matView_);
	matView_ = matViewInverse_;
	// 透視投影の生成
	matProjection_ = MyMathUtility::MakePerspective(fovAngleY, aspect, nearZ_, farZ_);
	// 定数バッファに転送
	TransferMatrix();
}

void Camera::UpdateMatrix()
{
	//回転、平行移動行列の計算
	matRot = MyMathUtility::MakeIdentity();
	matRot = MyMathUtility::MakeRotation(rotation_);
	matTrans = MyMathUtility::MakeTranslation(translation_);

	//ワールド行列の合成
	matCameraWorld_ = MyMathUtility::MakeIdentity();
	matCameraWorld_ *= matRot;
	matCameraWorld_ *= matTrans;

	//親行列の指定がある場合は、掛け算する
	if ( parent != nullptr )
	{
		matCameraWorld_ *= parent->matCameraWorld_;
	}

	//ビュー行列の逆行列
	matView_ = MyMathUtility::MakeInverse(matCameraWorld_);

	// 透視投影の生成
	matProjection_ = MyMathUtility::MakePerspective(fovAngleY,aspect,nearZ_,farZ_);
	// 定数バッファに転送
	TransferMatrix();
}

void Camera::UpdateQuaternion()
{
	//回転、平行移動行列の計算
	matRot = MyMathUtility::MakeIdentity();
	matRot = MyMathUtility::MakeQuaternion(quaternion_);
	matTrans = MyMathUtility::MakeTranslation(translation_);

	//ワールド行列の合成
	matCameraWorld_ = MyMathUtility::MakeIdentity();
	matCameraWorld_ *= matRot;
	matCameraWorld_ *= matTrans;

	//親行列の指定がある場合は、掛け算する
	if ( parent != nullptr )
	{
		matCameraWorld_ *= parent->matCameraWorld_;
	}

	//ビュー行列の逆行列
	matView_ = MyMathUtility::MakeInverse(matCameraWorld_);

	// 透視投影の生成
	matProjection_ = MyMathUtility::MakePerspective(fovAngleY,aspect,nearZ_,farZ_);
	// 定数バッファに転送
	TransferMatrix();
}

void Camera::TransferMatrix()
{
	// 定数バッファに書き込み
	constBuffMap->view = matViewInverse_;
	constBuffMap->projection = matProjection_;
	constBuffMap->cameraPos = eye_;
}

#pragma region Getter&Setter
const MyMath::Matrix4& Camera::GetMatView()
{
	return matView_;
}

const MyMath::Matrix4& Camera::GetMatViewInverse()
{
	return matViewInverse_;
}

const MyMath::Matrix4& Camera::GetMatProjection()
{
	return matProjection_;
}

const MyMath::Vector3& Camera::GetEye()
{
	return eye_;
}

const MyMath::Vector3& Camera::GetTarget()
{
	return target_;
}

const MyMath::Vector3& Camera::GetUp()
{
	return up_;
}

const MyMath::Vector3& Camera::GetTranslation()
{
	return translation_;
}

const MyMath::Vector3& Camera::GetRotation()
{
	return rotation_;
}

const MyMath::Matrix4& Camera::GetMatWorld()
{
	return matCameraWorld_;
}

MyMath::Matrix4& Camera::GetMatRot()
{
	return matRot;
}

void Camera::SetTranslation(MyMath::Vector3 translation)
{
	translation_ = translation;
}

void Camera::SetRotation(MyMath::Vector3 rotation)
{
	rotation_ = rotation;
}

void Camera::SetEye(const MyMath::Vector3& eye)
{
	eye_ = eye;
}

void Camera::SetTarget(const MyMath::Vector3& target)
{
	target_ = target;
}

void Camera::SetUp(const MyMath::Vector3& up)
{
	up_ = up;
}

void Camera::SetFarZ(const float& farZ)
{
	farZ_ = farZ;
}

void Camera::SetNearZ(const float& nearZ)
{
	nearZ_ = nearZ;
}
void Camera::SetDistance(const float& distance)
{
	distance_ = distance;
}
void Camera::SetFovAngleY(const float& fovAngle)
{
	fovAngleY = fovAngle;
}
float Camera::GetHAngle(const MyMath::Vector3& eye, const MyMath::Vector3& target)
{
	//注視点の向きベクトルを求める
	MyMath::Vector3 dir = target - eye;
	dir.normalize();

	//HAngle(XZ平面での角度)を求める
	float deg = MyMathUtility::Atan2(-dir.z, dir.x);
	deg *= MyMathUtility::degree2Radius;

	//調整角度(Z方向を基準に回転する)
	float adjustmentAngle = 90.0f;
	deg += adjustmentAngle;

	// -180～180に丸める
	if (deg > 180.0f)
	{
		deg -= 360.0f;
	}
	if (deg < -180.0f)
	{
		deg += 360.0f;
	}

	return deg;
}
float Camera::GetVAngle(const MyMath::Vector3& eye, const MyMath::Vector3& target)
{
	//注視点の向きベクトルを求める
	MyMath::Vector3 dir = target - eye;
	dir = dir.normalize();

	float fFront;
	{
		MyMath::Vector3 vFront;
		vFront.copy(vFront, dir);
		vFront.y = 0;
		fFront = vFront.length();
	}

	// Y軸とXZ平面の前方方向との角度を求める
	float deg = MyMathUtility::Atan2(-dir.z, dir.x);
	deg *= MyMathUtility::degree2Radius;

	// 可動範囲は -90～90
	if (deg > 90.0f)
	{
		deg = 180.0f - deg;
	}
	if (deg < -90.0f)
	{
		deg = -180.0f - deg;
	}

	return deg;
}

void Camera::SetCameraRot(MyMath::Vector3& rotation)
{
	MyMath::Vector3 oldTarget = target_;

	MyMath::Vector3 offset = { 0,rotation.x,0};
	
	offset = MyMath::Vec3Mat4Mul(offset, matView_);

	MyMath::Vector3 target = target_ + offset;

	target_ = target;
}
#pragma endregion
