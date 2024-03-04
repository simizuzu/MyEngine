#include "WorldTransform.h"
#include"DirectXCommon.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @file WorldTransform.cpp
 * @brief WorldTransformの処理について書いてあります
 */

void WorldTransform::Initialize()
{
	CreateConstBuffer();
}

void WorldTransform::Update(Camera* camera,bool billboradFlag)
{
	HRESULT result;
	MyMath::Matrix4 matScale,matRot,matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = MyMathUtility::MakeScaling(scale_);
	matRot = MyMathUtility::MakeIdentity();
	matRot = MyMathUtility::MakeRotation(rotation_);
	matTrans = MyMathUtility::MakeTranslation(translation_);

	// ワールド行列の合成
	matWorld = MyMathUtility::MakeIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//親行列の指定がある場合は、掛け算する
	if ( parent != nullptr )
	{
		matWorld *= parent->matWorld;
	}

	//親行列の指定がある場合は、掛け算する
	if ( parentMat != nullptr )
	{
		matWorld *= *parentMat;
	}

	const MyMath::Matrix4 matView = camera->GetMatView();
	const MyMath::Matrix4 matProjection = camera->GetMatProjection();
	const MyMath::Vector3& cameraPos = camera->GetEye();

	if ( billboradFlag )
	{
		const MyMath::Matrix4 matBillboard = camera->GetMatBillboard();

		matWorld = MyMathUtility::MakeIdentity();
		matWorld *= matScale;
		matWorld *= matRot;
		matWorld *= matBillboard;
		matWorld *= matTrans;
	}

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffer_->Map(0,nullptr,( void** ) &constMap);
	assert(SUCCEEDED(result));
	constMap->viewproj = matView * matProjection;
	constMap->world = matWorld;
	constMap->cameraPos = cameraPos;
	constBuffer_->Unmap(0,nullptr);
}

void WorldTransform::CreateConstBuffer()
{
	HRESULT result;

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = ( sizeof(ConstBufferDataB0) + 0xff ) & ~0xff;//頂点データ全体のサイズ
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
		IID_PPV_ARGS(&constBuffer_));
	assert(SUCCEEDED(result));
}


void WorldTransform::SetTranslation(MyMath::Vector3 translation)
{
	translation_ = translation;
}

void WorldTransform::SetScale(MyMath::Vector3 scale)
{
	scale_ = scale;
}

void WorldTransform::SetRotation(MyMath::Vector3 rotation)
{
	rotation_ = rotation;
}

MyMath::Vector3 WorldTransform::GetTranslation()
{
	return translation_;
}

MyMath::Vector3 WorldTransform::GetScale()
{
	return scale_;
}

MyMath::Vector3 WorldTransform::GetRotation()
{
	return rotation_;
}

void WorldTransform::MakeMatWorld()
{
	MyMath::Matrix4 matScale,matRot,matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = MyMathUtility::MakeScaling(scale_);
	matRot = MyMathUtility::MakeIdentity();
	matRot = MyMathUtility::MakeRotation(rotation_);
	matTrans = MyMathUtility::MakeTranslation(translation_);

	// ワールド行列の合成
	matWorld = MyMathUtility::MakeIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;
}

D3D12_GPU_VIRTUAL_ADDRESS WorldTransform::GetGpuAddress()
{
	return constBuffer_->GetGPUVirtualAddress();
}

namespace MyMath
{
	MyMath::Vector3 GetWorldPosition(const WorldTransform& transform)
	{
		// ワールド座標を入れる変数
		Vector3 worldPos;
		// ワールド行列の平行移動成分を取得
		worldPos.x = transform.matWorld.m[ 3 ][ 0 ];
		worldPos.y = transform.matWorld.m[ 3 ][ 1 ];
		worldPos.z = transform.matWorld.m[ 3 ][ 2 ];

		return worldPos;
	}
}