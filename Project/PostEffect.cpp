#include "PostEffect.h"

#include "DirectXCommon.h"
#include "Input.h"
#pragma comment(lib,"d3dcompiler.lib")

#pragma warning(push)
#include "DirectXTex.h"
#include <WinApp.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <memory>
#pragma warning(pop)

Microsoft::WRL::ComPtr<ID3D12Device> PostEffect::device_;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> PostEffect::cmdList_;
const float PostEffect::clearColor[4] = { 0.0f,0.3f,0.0f,1.0f };
RootsigSetPip PostEffect::pipline_;

void PostEffect::CreateGraphicsPipeline()
{
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;	// 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト

	Shader::CreatePostEffectShade(vsBlob, psBlob);
	Pipeline::CreatePostEffectPipeline(vsBlob.Get(), psBlob.Get(), device_.Get(), pipline_);
}

void PostEffect::Initialize(ID3D12Device* device)
{
	device_ = device;
	assert(device_);
	CreateVertexBuffer();
	CreateTex();
	CreateSRV();
	CreateRTV();
	CreateDepth();
	CreateDSV();
	CreateGraphicsPipeline();
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList_)
{
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList_->SetPipelineState(pipline_.pipelineState.Get());
	cmdList_->SetGraphicsRootSignature(pipline_.rootSignature.Get());
	// プリミティブ形状の設定コマンド
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
	// 頂点バッファビューの設定コマンド
	cmdList_->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー（CBV）の設定コマンド
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//定数バッファビュー(SRV)の設定コマンド
	//D3D12_GPU_DESCRIPTOR_HANDLE descHeapSRVHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();

	// 描画コマンド
	CD3DX12_GPU_DESCRIPTOR_HANDLE descSrvGPUHandle1 =
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0,
			device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	cmdList_->SetGraphicsRootDescriptorTable(1, descSrvGPUHandle1);

	CD3DX12_GPU_DESCRIPTOR_HANDLE descSrvGPUHandle2 =
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
			device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	cmdList_->SetGraphicsRootDescriptorTable(2, descSrvGPUHandle2);

	cmdList_->DrawInstanced(_countof(vertices_), 1, 0, 0);//全ての頂点を使って描画
}

void PostEffect::CreateTex()
{
	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::GetInstance()->window_width,
		(int32_t)WinApp::GetInstance()->window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//テクスチャバッファの生成
	CD3DX12_HEAP_PROPERTIES texHeap = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	for (int i = 0; i < 2; i++)
	{
		result_ = device_->CreateCommittedResource(
			&texHeap,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(&texBuff[i]));
		assert(SUCCEEDED(result_));

		{//テクスチャを赤クリア
			//画素数(1280*720=921600ピクセル)
			const uint32_t pixelCount = WinApp::GetInstance()->window_width * WinApp::GetInstance()->window_height;
			//画像1行分のデータサイズ
			const uint32_t rowPitch = sizeof(uint32_t) * WinApp::GetInstance()->window_width;
			//画像1行分のデータサイズ
			const uint32_t depthPitch = sizeof(uint32_t) * WinApp::GetInstance()->window_height;
			//画像イメージ
			uint32_t* img = new uint32_t[pixelCount];
			for (size_t j = 0; j < pixelCount; j++)
			{
				img[j] = 0xff0000ff;
			}

			//テクスチャバッファデータにデータ転送
			result_ = texBuff[i]->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
			assert(SUCCEEDED(result_));
			delete[]img;
		}
	}
}

void PostEffect::CreateSRV()
{
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;
	//ＳＲＶ用デスクリプタヒープを生成
	result_ = device_->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < 2; i++)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvDescHandle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapSRV->GetCPUDescriptorHandleForHeapStart(), i,
				device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

		//デスクリプタヒープにSRV作成
		device_->CreateShaderResourceView(texBuff[i].Get(), &srvDesc, srvDescHandle);
	}
}

void PostEffect::CreateRTV()
{
	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;
	//RTV用デスクリプタヒープを生成
	result_ = device_->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result_));

	//レンダーゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (int i = 0; i < 2; i++)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE descHandleRTV =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		//デスクリプタヒープにRTV作成
		device_->CreateRenderTargetView(texBuff[i].Get(), nullptr, descHandleRTV);
	}
}

void PostEffect::CreateDepth()
{
	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, WinApp::GetInstance()->window_width, WinApp::GetInstance()->window_height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//深度バッファの生成
	CD3DX12_HEAP_PROPERTIES depthProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE depthVale = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result_ = device_->CreateCommittedResource(
		&depthProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthVale,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result_));
}

void PostEffect::CreateDSV()
{
	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result_ = device_->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result_));
	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreateVertexBuffer()
{
	//頂点バッファの生成
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * 4);
	result_ = device_->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result_));

	//頂点データ
	Vertex vertices[4] = {
		{{-0.5f,-0.5f,0.0f},{0.0f,1.0f}},//左下
		{{-0.5f,+0.5f,0.0f},{0.0f,0.0f}},//左上
		{{+0.5f,-0.5f,0.0f},{1.0f,1.0f}},//右下
		{{+0.5f,+0.5f,0.0f},{1.0f,0.0f}},//右上
	};

	//頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	result_ = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result_))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(Vertex) * 4;
	vbView.StrideInBytes = sizeof(Vertex);

	//定数バッファの生成
	resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataPE) + 0xff) & ~0xff);
	result_ = device_->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result_));

	//定数バッファにデータ転送
	result_ = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result_))
	{
		constMap->color = color;
		constMap->mat = MyMathUtility::MakeIdentity();
		constBuff->Unmap(0, nullptr);
	}
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList_)
{
	for (int i = 0; i < 2; i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBuff =
			CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);
		//リソースバリアを変更（シェーダーリソース→描画可能）
		cmdList_->ResourceBarrier(
			1, &resourceBuff);
	}
	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];
	for (int i = 0; i < 2; i++)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHsDescHandle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		rtvHs[i] = rtvHsDescHandle;
	}
	descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList_->OMSetRenderTargets(2, rtvHs, false, &dsvH);
	//ビューポートの設定
	CD3DX12_VIEWPORT viewPorts[2];
	CD3DX12_RECT scissorRects[2];
	for (int i = 0; i < 2; i++)
	{
		viewPorts[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<LONG>(WinApp::GetInstance()->window_width), static_cast<LONG>(WinApp::GetInstance()->window_height));
		scissorRects[i] = CD3DX12_RECT(0, 0, static_cast<LONG>(WinApp::GetInstance()->window_width), static_cast<LONG>(WinApp::GetInstance()->window_height));
	}

	cmdList_->RSSetViewports(2, viewPorts);
	cmdList_->RSSetScissorRects(2, scissorRects);

	for (int i = 0; i < 2; i++)
	{
		//全画面クリア
		cmdList_->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}
	//深度バッファのクリア
	cmdList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList_)
{
	for (int i = 0; i < 2; i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBuff =
			CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		//リソースバリアを変更(描画可能→シェーダーリソース)
		cmdList_->ResourceBarrier(1, &resourceBuff);
	}
}