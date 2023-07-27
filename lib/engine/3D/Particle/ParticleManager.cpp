#include "ParticleManager.h"
#include "EngineUtility.h"
#include "DirectXCommon.h"
#include <cassert>
#include"TextureManager.h"
#include"Shader.h"

#pragma warning(push)
#include <DirectXTex/d3dx12.h>
#pragma warning(disable: 4091)
#pragma warning(pop)

/// <summary>
/// 静的メンバ変数の実態
/// </summary>
Microsoft::WRL::ComPtr<ID3D12Device> ParticleManager::device_;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> ParticleManager::cmdList_;
RootsigSetPip ParticleManager::pip;

void ParticleManager::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	assert(device);
	ParticleManager::device_ = device;
	ParticleManager::cmdList_ = cmdList;
	CreateCraphicsPipeline();
}


void ParticleManager::Initialize()
{
	InitializeDescriptorHeap();
	CreateModel();
}

void ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void ParticleManager::CreateCraphicsPipeline()
{
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob;

	Shader::CreateParticleShade(vsBlob, psBlob, gsBlob);
	Pipeline::CreatePaticlePipeline(vsBlob.Get(), psBlob.Get(), gsBlob.Get(), device_.Get(), pip);
}

void ParticleManager::Update()
{
	HRESULT result;

	// 寿命が尽きたパーティクルを全削除
	particles.remove_if([](Particle& x) {return x.frame >= x.num_frame; });

	// 全パーティクル更新
	for (std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end();
		it++) {
		// 経過フレーム数をカウント
		it->frame++;
		// 速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		// 速度による移動
		it->position = it->position + it->velocity;

		// 進行度を0～1の範囲に換算
		float f = (float)it->frame / (float)it->num_frame;
		// スケールの線形補間
		it->scale = (it->e_scale - it->s_scale) * f;
		it->scale += it->s_scale;

		//赤の線形補間
		it->color.x = (it->e_color.x - it->s_color.x) * f;
		it->color.x += it->s_color.x;
		//青の線形補間
		it->color.y = (it->e_color.y - it->s_color.y) * f;
		it->color.y += it->s_color.y;
		//緑の線形補間
		it->color.z = (it->e_color.z - it->s_color.z) * f;
		it->color.z += it->s_color.z;
	}

	// 頂点バッファへデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	uint16_t count = 0;
	if (SUCCEEDED(result)) {
		// パーティクルの情報を一つずつ反映
		for (std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end();
			it++) {

			if (count == vertexCount)
			{
				break;
			}
			// 座標
			vertMap->pos = it->position;
			// スケール
			vertMap->scale = it->scale;
			// 色
			vertMap->color = it->color;
			// 次の頂点へ
			vertMap++;

			count++;
		}
		vertBuff->Unmap(0, nullptr);
	}
}

void ParticleManager::Add(int life, MyMath::Vector3 position, MyMath::Vector3 velocity, MyMath::Vector3 accel, float start_scale, float end_scale, MyMath::Vector4 s_color, MyMath::Vector4 e_color)
{
	//リストに要素を追加
	particles.emplace_front();
	//追加した要素の参照
	Particle& p = particles.front();
	//値のセット
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.scale = start_scale;
	p.s_scale = start_scale;
	p.e_scale = end_scale;

	p.s_color = s_color;
	p.e_color = e_color;
}

void ParticleManager::RandParticle()
{
	for (int i = 0; i < 30; i++)
	{
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 10.0f;
		MyMath::Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.3f;
		MyMath::Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		// 重力に見立ててYのみ[-0.001f,0]でランダムに分布
		MyMath::Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		MyMath::Vector4 col{};
		const float rnd_col = 1.0f;
		col.x = (float)rand() / RAND_MAX * rnd_col;
		col.y = (float)rand() / RAND_MAX * rnd_col;
		col.z = (float)rand() / RAND_MAX * rnd_col;

		// 追加
		Add(60, pos, vel, acc, 1.0f, 0.0f, { 1,0,1,1 }, { 1,0,1,1 });
	}
}

void ParticleManager::Draw(WorldTransform* transform)
{
	// nullptrチェック
	assert(device_);
	assert(ParticleManager::cmdList_);

	// 頂点バッファの設定
	cmdList_->IASetVertexBuffers(0, 1, &vbView);

		// パイプラインステートの設定
	cmdList_->SetPipelineState(pip.pipelineState.Get());
	// ルートシグネチャの設定
	cmdList_->SetGraphicsRootSignature(pip.rootSignature.Get());
	// プリミティブ形状を設定
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(0, transform->GetGpuAddress());
	// シェーダリソースビューをセット
	cmdList_->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// 描画コマンド_countof(indices)
		// 描画コマンド
	if (std::distance(particles.begin(), particles.end()) < vertexCount)
	{
		cmdList_->DrawInstanced(static_cast<UINT> (std::distance(particles.begin(), particles.end())), 1, 0, 0);
	}
	else
	{
		cmdList_->DrawInstanced(vertexCount, 1, 0, 0);
	}
}


void ParticleManager::LoadTexture(const wchar_t* texturename)
{
	HRESULT result = S_FALSE;

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};

	result = LoadFromWICFile(texturename, DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	DirectX::ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);


	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// テクスチャ用バッファの生成
	result = device_->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr, IID_PPV_ARGS(&texbuff));
	assert(SUCCEEDED(result));


	// テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const DirectX::Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
		result = texbuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device_->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV
	);
}

void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(vertices));

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = device_->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}
