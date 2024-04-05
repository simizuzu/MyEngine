#include "Sprite.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <d3dcompiler.h>
MYENGINE_SUPPRESS_WARNINGS_END

#pragma comment(lib,"d3dcompiler.lib")

 /**
 * @file Sprite.cpp
 * @brief Spriteの処理について書いてあります
 */

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Sprite::device_;
UINT Sprite::descriptorSize_;
ID3D12GraphicsCommandList* Sprite::commandList_;
ID3D12RootSignature* Sprite::rootSignature_;
std::array<RootsigSetPip, 6> Sprite::pipelineState;
MyMath::Matrix4 Sprite::matProjection_;

void Sprite::StaticInitialize()
{
	device_ = DirectXCommon::GetInstance()->GetDevice();
	commandList_ = DirectXCommon::GetInstance()->GetCommandList();

	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;

	float width = static_cast<float>(WinApp::GetInstance()->window_width);
	float height = static_cast<float>(WinApp::GetInstance()->window_height);

	// デスクリプタサイズを取得
	descriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 平行投影
	MyMathUtility::MakeOrthogonalL(0.0f, width, height, 0.0f, 0.0f, 1.0f, matProjection_);

	// シェーダーの読み込み
	Shader::CreateSpriteShade(vsBlob, psBlob);

	for (size_t i = 0; i < pipelineState.size(); i++)
	{
		Pipeline::CreateSpritePipeline(vsBlob.Get(), psBlob.Get(), (BlendMode)i, device_, pipelineState);
	}
}

void Sprite::SetPosiotion(const MyMath::Vector2& position)
{
	position_ = position;
}

void Sprite::SetRotation(float rotation)
{
	rotation_ = rotation;
}

void Sprite::SetSize(const MyMath::Vector2& size)
{
	size_ = size;
}

void Sprite::SetColor(const MyMath::Vector4& color)
{
	color_ = color;
}

void Sprite::Initialize()
{
	CreateVertexIndexBuff();
	CreateConstBuff();
}

void Sprite::Update(const MyMath::Vector2 pos, const MyMath::Vector2 scale, float rot)
{
	MyMath::Matrix4 mTrans, mRot, mScale, matWorld;
	mTrans = MyMathUtility::MakeTranslation({ pos.x,pos.y,0.0f });
	mRot = MyMathUtility::MakeRotation({ 0.0f,0.0f,rot });
	mScale = MyMathUtility::MakeScaling({ scale.x,scale.y,1.0f });
	matWorld = mScale * mRot * mTrans;

	*constBuffMap = matWorld * matProjection_;
}

void Sprite::Draw(TextureData& textureData, MyMath::Vector2 position, MyMath::Vector2 scale, float rotation,
	MyMath::Vector2 anchorpoint, bool flipX, bool flipY)
{
	int isFlipX, isFlipY;
	if (flipX == false)isFlipX = 1;
	else isFlipX = -1;
	if (flipY == false)isFlipY = 1;
	else isFlipY = -1;

	float left = ((0.0f - anchorpoint.x) * (float)textureData.width) * ( float ) isFlipX;
	float right = ((1.0f - anchorpoint.x) * ( float ) textureData.width) * ( float ) isFlipX;
	float top = ((0.0f - anchorpoint.y) * ( float ) textureData.height) * ( float ) isFlipY;
	float bottom = ((1.0f - anchorpoint.y) * ( float ) textureData.height) * ( float ) isFlipY;

	//頂点データ
	PosUvColor vertices[] =
	{
		{{left,top,0.0f},{0.0f,0.0f},color_},//左上インデックス0
		{{left,bottom,0.0f},{0.0f,1.0f},color_},//左下インデックス1
		{{right,top,0.0f},{1.0f,0.0f},color_},//右上インデックス2
		{{right,bottom,0.0f},{1.0f,1.0f},color_},//右下インデックス3
	};

	//インデックスデータ
	uint16_t indices[] =
	{
		1,0,3,//三角形1つ目
		2,3,0,//三角形2つ目
	};

	//頂点バッファへのデータ転送
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];//インデックスをコピー
	}

	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];//インデックスをコピー
	}

	Update(position, scale, rotation);

	SetBlendMode((BlendMode)blendMode);

	DrawCommandList(textureData);

	// 描画コマンド
	commandList_->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void Sprite::Finalize()
{
	for ( size_t i = 0; i < pipelineState.size(); i++ )
	{
		pipelineState[ i ].pipelineState.Reset();
		pipelineState[ i ].rootSignature.Reset();
	}
}

void Sprite::DrawCommandList(TextureData textureData)
{
	// プリミティブ形状の設定コマンド
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
	// 頂点バッファビューの設定コマンド
	commandList_->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー(CBV)の設定コマンド
	commandList_->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//SRVヒープの設定コマンド
	commandList_->SetDescriptorHeaps(1, textureData.srvHeap.GetAddressOf());
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureData.srvGpuHandle;
	//SRVヒープ先頭にあるSRVをルートパラメーター1番に設定
	commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	//インデックスバッファビューの設定コマンド
	commandList_->IASetIndexBuffer(&ibView);
}

void Sprite::CreateVertexIndexBuff()
{
	HRESULT result;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	//頂点データ全体のサイズ = 頂点データサイズ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(PosUvColor) * 4);

	//頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	result = device_->CreateCommittedResource(
		&heapProp,//リソース設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//頂点バッファビューの作成
	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(PosUvColor);

	//インデックスデータのサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * 6);
	//頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定　
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
	result = device_->CreateCommittedResource(
		&heapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(indexBuff.ReleaseAndGetAddressOf()));

	//インデックスバッファのマッピング
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
	assert(SUCCEEDED(result));
}

void Sprite::CreateConstBuff()
{
	HRESULT result;

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(MyMath::Matrix4) + 0xff) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device_->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constBuff.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuff->Map(0, nullptr, (void**)&constBuffMap);//マッピング
	assert(SUCCEEDED(result));
}

void Sprite::SetTextureIndex(uint32_t textureIndex)
{
	textureIndex_ = textureIndex;
}

void Sprite::SetBlendMode(BlendMode mode)
{
	switch (mode)
	{
	case BlendMode::None:

		commandList_->SetPipelineState(pipelineState[0].pipelineState.Get());
		commandList_->SetGraphicsRootSignature(pipelineState[0].rootSignature.Get());

		break;

	case BlendMode::Alpha:

		commandList_->SetPipelineState(pipelineState[1].pipelineState.Get());
		commandList_->SetGraphicsRootSignature(pipelineState[1].rootSignature.Get());

		break;

	case BlendMode::Add:

		commandList_->SetPipelineState(pipelineState[2].pipelineState.Get());
		commandList_->SetGraphicsRootSignature(pipelineState[2].rootSignature.Get());

		break;

	case BlendMode::Sub:

		commandList_->SetPipelineState(pipelineState[3].pipelineState.Get());
		commandList_->SetGraphicsRootSignature(pipelineState[3].rootSignature.Get());

		break;

	case BlendMode::Mul:

		commandList_->SetPipelineState(pipelineState[4].pipelineState.Get());
		commandList_->SetGraphicsRootSignature(pipelineState[4].rootSignature.Get());

		break;

	case BlendMode::Inv:

		commandList_->SetPipelineState(pipelineState[5].pipelineState.Get());
		commandList_->SetGraphicsRootSignature(pipelineState[5].rootSignature.Get());

		break;
	}
}

const MyMath::Vector2& Sprite::GetPosition() const
{
	return position_;
}
float Sprite::GetRotation() const
{
	return rotation_;
}

const MyMath::Vector4& Sprite::GetColor() const
{
	return color_;
}

const MyMath::Vector2& Sprite::GetSize() const
{
	return size_;
}

uint32_t Sprite::GetTextureIndex()
{
	return textureIndex_;
}
