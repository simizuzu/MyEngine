#include "ObjObject3d.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <windows.h>
MYENGINE_SUPPRESS_WARNINGS_END

#include "DirectX12Math.h"
#include "ModelManager.h"

 /**
 * @file ObjObject3d.cpp
 * @brief ObjObject3dの処理について書いてあります
 */

/// <summary>
/// 静的メンバ変数の実態
/// </summary>
ID3D12Device* ObjObject3d::device_;
ID3D12GraphicsCommandList* ObjObject3d::cmdList_;
RootsigSetPip ObjObject3d::pip;
Light* ObjObject3d::light_ = nullptr;

void ObjObject3d::StaticInitialize(ID3D12Device* device)
{
	// nullptrチェック
	assert(device);

	ObjObject3d::device_ = device;

	// グラフィックスパイプラインの生成
	CreateGraphicsPipeline();

	ObjModel::SetDevice(device);
}

void ObjObject3d::CreateGraphicsPipeline()
{
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト

	Shader::CreateObjShade(vsBlob, psBlob);

	Pipeline::CreateObjPipeline(vsBlob.Get(), psBlob.Get(), BlendMode::None, device_,pip);
}

ObjObject3d* ObjObject3d::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	ObjObject3d* object3d = new ObjObject3d();
	if (object3d == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize()) {
		delete object3d;
		assert(0);
		return nullptr;
	}

	return object3d;
}

bool ObjObject3d::Initialize()
{
	HRESULT result;

	//定数バッファの生成
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device_->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));
	assert(SUCCEEDED(result));

	return true;
}

void ObjObject3d::Draw(WorldTransform* transform)
{
	cmdList_ = DirectXCommon::GetInstance()->GetCommandList();

	// nullチェック
	assert(device_);
	assert(ObjObject3d::cmdList_);

	// モデルの割り当てがなければ描画しない
	if (model_ == nullptr)
	{
		return;
	}

	// パイプラインステートの設定
	cmdList_->SetPipelineState(pip.pipelineState.Get());
	// ルートシグネチャの設定
	cmdList_->SetGraphicsRootSignature(pip.rootSignature.Get());
	// プリミティブ形状の設定コマンド
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
	// 定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(2, transform->GetGpuAddress());

	//ライトの描画
	light_->Draw(cmdList_, 3);
	// モデル描画
	model_->Draw(cmdList_);
}

void ObjObject3d::SetModel(ObjModel* model)
{
	model_ = model;
}

void ObjObject3d::SetModel(const std::string& filePath,bool smoothing)
{
	model_ = ModelManager::GetInstance()->FindObjModel(filePath);

	if ( !model_ )
	{
		ModelManager::GetInstance()->LoadModel(filePath,obj,smoothing);
		model_ = ModelManager::GetInstance()->FindObjModel(filePath);
	}
}

void ObjObject3d::Finalize()
{
	device_ = nullptr;
	cmdList_ = nullptr;
	pip.pipelineState.Reset();
	pip.rootSignature.Reset();
}

void ObjObject3d::SetLight(Light* light)
{
	ObjObject3d::light_ = light;
}

namespace MyMath
{
	Matrix4 MakeWorldMatrix4(ObjObject3d& transform)
	{
		Matrix4 matWorld = MyMathUtility::MakeIdentity();
		Matrix4 matScale, matRot, matTrans;

		// スケーリング倍率
		matScale = MyMathUtility::MakeScaling(transform.scale);
		// 回転行列
		matRot = MyMathUtility::MakeRotation(transform.rotation);
		// 移動量
		matTrans = MyMathUtility::MakeTranslation(transform.position);

		// matWorldに掛け算
		matWorld = matScale * matRot * matTrans;

		if (transform.parent)
		{
			matWorld *= transform.parent->matWorld;
		}

		return matWorld;
	}
}
