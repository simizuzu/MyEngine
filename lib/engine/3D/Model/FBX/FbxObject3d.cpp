#include "FbxObject3d.h"

#include "DirectXCommon.h"
#include "Shader.h"

//静的メンバ変数の実態
Microsoft::WRL::ComPtr<ID3D12Device> FbxObject3d::device_;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> FbxObject3d::cmdList_;
RootsigSetPip FbxObject3d::pip;

void FbxObject3d::StaticInitialize(ID3D12Device* device)
{
	assert(device);

	FbxObject3d::device_ = device;

	//グラフィックスパイプラインの生成
	CrateGrapicsPipeline();

}

bool FbxObject3d::Initialize()
{
	HRESULT result;

	CD3DX12_HEAP_PROPERTIES HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC RESOURCE_DESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransformFbx) + 0xff) & ~0xff);

	result = device_->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);

	return true;
}

FbxObject3d* FbxObject3d::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	FbxObject3d* object3d = new FbxObject3d();
	if (object3d == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize())
	{
		delete object3d;
		assert(0);
		return nullptr;
	}

	return object3d;
}

void FbxObject3d::Draw(WorldTransform* transform)
{
	cmdList_ = DirectXCommon::GetInstance()->GetCommandList();

	//モデルの割り当てがなければ描画しない
	if (model_ == nullptr)
	{
		return;
	}

	//パイプラインステートの設定
	cmdList_->SetPipelineState(pip.pipelineState.Get());
	//ルートシグネチャの設定
	cmdList_->SetGraphicsRootSignature(pip.rootSignature.Get());
	//プリミティブ形状を設定
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(0, transform->GetGpuAddress());

	//モデル描画
	model_->Draw(cmdList_.Get());
}

void FbxObject3d::SetModel(FbxModel* model)
{
	model_ = model;
}

void FbxObject3d::CrateGrapicsPipeline()
{
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト

	Shader::CreateFBXShade(vsBlob,psBlob);

	Pipeline::CreateFBXPipeline(vsBlob.Get(),psBlob.Get(),device_.Get(), pip);
}