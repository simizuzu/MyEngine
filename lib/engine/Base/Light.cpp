#include "Light.h"
#include <cassert>

/// <summary>
/// 静的メンバ変数の実態
/// </summary>
DirectXCommon* Light::device_ = nullptr;

void Light::StaticInitialise(DirectXCommon* device)
{
	assert(!device_);
	assert(device);
	device_ = device;
}

Light* Light::Create()
{
	//3Dオブジェクトのインスタンスを生成
	Light* instance = new Light();
	//初期化
	instance->Initialize();
	//生成したインスタンスを返す
	return instance;
}

void Light::Initialize()
{
	HRESULT result;
	//定数バッファの生成
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	//定数バッファへデータ転送
	TransferConstBuffer();
}

void Light::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootPramIndex)
{
	cmdList->SetGraphicsRootConstantBufferView(rootPramIndex,constBuff->GetGPUVirtualAddress());
}

void Light::TransferConstBuffer()
{
	HRESULT result;

	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->lightVec = -lightdir;
		constMap->lightColor = lightColor;
		constBuff->Unmap(0, nullptr);
	}
}

void Light::SetLightDir(const MyMath::Vector4& lightdir)
{
	//正規化してセット
	this->lightdir.x = lightdir.x;
	this->lightdir.y = lightdir.y;
	this->lightdir.z = lightdir.z;
	dirty = true;
}

void Light::SetLightColor(const MyMath::Vector3& lightColor)
{
	this->lightColor = lightColor;
	dirty = true;
}

