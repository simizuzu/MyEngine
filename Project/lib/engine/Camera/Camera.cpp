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

ViewProjection viewPro;
ConstBufferDataViewProjection ConstMap;

void Camera::Initialize()
{
	eye_ = { 0,0,-distance_ };//視点座標
	target_ = { 0,0,0 };//注視点座標
	up_ = { 0,1,0 };//上方向ベクトル

	nearZ_ = 0.1f;
	farZ_ = 1000.0f;

	// アスペクト比を計算(画面横幅/画面縦幅)
	aspect = 
		static_cast<float>(WinApp::GetInstance()->window_width) /
		static_cast<float>(WinApp::GetInstance()->window_height);

	CreateConstBuffer();
	Map();
	UpdateLookAt();
}

void Camera::Update(bool isMatrix)
{
	if ( isMatrix )
	{
		UpdateMatrix();
	}
	else
	{
		UpdateLookAt();
	}
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
		IID_PPV_ARGS(viewPro.constBuff_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
}

void Camera::Map()
{
	HRESULT result;

	//定数バッファのマッピング
	result = viewPro.constBuff_->Map(0, nullptr, (void**)&viewPro.constBuffMap);//マッピング
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
	//カメラのワールド行列（スケールは無し）
	MyMath::Matrix4 matTrans,matRot;

	//回転、平行移動行列の計算
	matRot = MyMathUtility::MakeIdentity();
	matRot = MyMathUtility::MakeRotation(rotation_);
	matTrans = MyMathUtility::MakeTranslation(translation_);

	//ワールド行列の合成
	matCameraWorld_ = MyMathUtility::MakeIdentity();
	matCameraWorld_ *= matRot;
	matCameraWorld_ *= matTrans;

	matView_ = MyMathUtility::MakeInverse(matCameraWorld_);

	// 透視投影の生成
	matProjection_ = MyMathUtility::MakePerspective(fovAngleY,aspect,nearZ_,farZ_);
	// 定数バッファに転送
	TransferMatrix();
}

void Camera::TransferMatrix()
{
	// 定数バッファに書き込み
	ConstMap.view = matViewInverse_;
	ConstMap.projection = matProjection_;
	ConstMap.cameraPos = eye_;
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
