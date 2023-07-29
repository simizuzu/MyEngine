#include "PostEffect.h"
#include"Sprite.h"
#include<cassert>

#pragma warning(push)
#pragma warning(disable: 4091)
#include <DirectXTex/d3dx12.h>
#pragma warning(pop)

const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };
RootsigSetPip PostEffect::pipline_;

PostEffect* PostEffect::Create()
{
	PostEffect* instance = new PostEffect();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) 
	{
		delete instance;
		assert(0);
		return nullptr;
	}
	return instance;
}

bool PostEffect::Initialize()
{
	device_ = dxCommon->GetInstance()->GetDevice();

	//パイプライン生成
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
	Shader::CreatePostEffectShade(vsBlob, psBlob);
	Pipeline::CreatePostEffectPipeline(vsBlob.Get(), psBlob.Get(), device_.Get(), pipline_);

	CreateVertexData(device_.Get());
	CreateTextureBuff(device_.Get());
	CreateSRVDesc(device_.Get());
	CreateRTVDesc(device_.Get());
	CreateDepthBuff(device_.Get());
	CreateDSVDesc(device_.Get());

	return false;
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	ConstBufferDataPE* constMap = nullptr;
	constBuff->Map(0,nullptr,(void**)&constMap);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };

	cmdList->SetDescriptorHeaps(1, ppHeaps);

	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//リソースバリアを変更（シェーダーリソース→描画可能）
	CD3DX12_RESOURCE_BARRIER CHANGE_RENDER_TARGET =
		CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdList->ResourceBarrier(1, &CHANGE_RENDER_TARGET);

	//レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//ビューポートの設定
	CD3DX12_VIEWPORT VIEWPORT =
		CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
	cmdList->RSSetViewports(1, &VIEWPORT);
	//シザリング矩形の設定
	CD3DX12_RECT RECT =
		CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height);
	cmdList->RSSetScissorRects(1, &RECT);

	//全画面クリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//リソースバリアを変更（描画可能→シェーダーリソース）
	CD3DX12_RESOURCE_BARRIER CHANGE_PIXEL_SHADER_RESOURCE =
		CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	cmdList->ResourceBarrier(1, &CHANGE_PIXEL_SHADER_RESOURCE);
}

void PostEffect::CreateVertexData(ID3D12Device* device)
{
	HRESULT result;

	CD3DX12_HEAP_PROPERTIES heapprop01 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDesc01 = CD3DX12_RESOURCE_DESC::Buffer(sizeof(PosUvColor) * 4);

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapprop01,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc01,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//頂点データ(ペラポリゴン)
	VertexPosUv vertices[] = {
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
		{{-0.5f, +0.5f, 0.0f}, {0.0f, 0.0f}},
		{{+0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}},
		{{+0.5f, +0.5f, 0.0f}, {1.0f, 0.0f}},
	};

	//頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0,nullptr);
	}

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	CD3DX12_HEAP_PROPERTIES heapprop02 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDesc02 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataPE) + 0xff) & ~0xff);

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&heapprop02, D3D12_HEAP_FLAG_NONE,
		&resourceDesc02,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}

void PostEffect::CreateTextureBuff(ID3D12Device* device)
{
	HRESULT result;

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texturesDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		winApp_->GetInstance()->window_width,
		winApp_->GetInstance()->window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//テクスチャバッファの生成
	CD3DX12_HEAP_PROPERTIES HEAP_PROPERTIES =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	CD3DX12_CLEAR_VALUE CLEAR_VALUE = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	result = device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&texturesDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CLEAR_VALUE,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	{	//テクスチャを赤クリア
		//画像数(1280 * 720)
		const uint32_t pixelCount = winApp_->GetInstance()->window_width * winApp_->GetInstance()->window_height;
		//画像1行分のデータサイズ
		const uint32_t rowPitch = sizeof(uint32_t) * winApp_->GetInstance()->window_width;
		//画像全体のデータサイズ
		const uint32_t depthPitch = rowPitch * winApp_->GetInstance()->window_height;
		//画像イメージ
		uint32_t* img = new uint32_t[pixelCount];
		for (uint32_t i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}
}

void PostEffect::CreateSRVDesc(ID3D12Device* device)
{
	HRESULT result;
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	device->CreateShaderResourceView(texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::CreateRTVDesc(ID3D12Device* device)
{
	HRESULT result;
	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc = {};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//デスクリプタヒープ二RTV作成
	device->CreateRenderTargetView(texBuff.Get(),
		&renderTargetViewDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::CreateDSVDesc(ID3D12Device* device)
{
	
}

void PostEffect::CreateDepthBuff(ID3D12Device* device)
{
	HRESULT result;
	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			winApp_->GetInstance()->window_width,
			winApp_->GetInstance()->window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	//深度バッファの生成
	CD3DX12_HEAP_PROPERTIES HEAP_PROPERTIES = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE CLEAR_VALUE = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CLEAR_VALUE,
		IID_PPV_ARGS(&depthBuff));

	assert(SUCCEEDED(result));
	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}
