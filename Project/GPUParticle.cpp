#include "GPUParticle.h"
#include "DirectXCommon.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <d3dx12.h>
#include <cassert>
#include <d3dcompiler.h>
MYENGINE_SUPPRESS_WARNINGS_END

void GPUParticle::Initialize(size_t MAX_PARTICLE)
{
	HRESULT result;
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	emitData.MAX_PARTICLE = ( uint32_t )MAX_PARTICLE;

	const uint32_t alignment = 4096;
	size_t deadListByteSize = sizeof(int32_t) * MAX_PARTICLE;
	size_t countBufferOffset = ( size_t ) ( (uint32_t)deadListByteSize + ( alignment - 1 ) ) & ~( alignment - 1 );

	{
	// デスクリプタヒープの設定
		D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
		descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
		descriptorHeapDesc.NumDescriptors = 10;
		// 設定を元にSRV用デスクリプタヒープを生成
		result = device->CreateDescriptorHeap(&descriptorHeapDesc,IID_PPV_ARGS(&descHeap));
		assert(SUCCEEDED(result));
	}

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHadle = descHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHadle = descHeap->GetGPUDescriptorHandleForHeapStart();

	//バッファの設定
	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);//ヒープ設定
	//リソース設定
	D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(int32_t) + countBufferOffset,D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	//DrawListの作成
	{
		//ドローリストの生成
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(&drawList_));
		assert(SUCCEEDED(result));

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = ( UINT ) MAX_PARTICLE;
		uavDesc.Buffer.StructureByteStride = sizeof(int32_t);
		uavDesc.Buffer.CounterOffsetInBytes = countBufferOffset;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

		device->CreateUnorderedAccessView(drawList_.Get(),drawList_.Get(),&uavDesc,cpuHadle);

		gpuHadle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cpuHadle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	//FreeListの作成
	{
		//フリーリストの生成
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(&freeList_));
		assert(SUCCEEDED(result));

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = ( UINT ) MAX_PARTICLE;
		uavDesc.Buffer.StructureByteStride = sizeof(int32_t);
		uavDesc.Buffer.CounterOffsetInBytes = countBufferOffset;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

		freeListUavHandle = gpuHadle;
		device->CreateUnorderedAccessView(freeList_.Get(),freeList_.Get(),&uavDesc,cpuHadle);
		gpuHadle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cpuHadle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	//PrticlePoolの作成
	{
		D3D12_RESOURCE_DESC resDesc2 = CD3DX12_RESOURCE_DESC::Buffer(sizeof(ParticleData) * MAX_PARTICLE,D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		//パーティクルプールの生成
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc2,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(&particlePool_));
		assert(SUCCEEDED(result));

		D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
		lUavResDesc.Format = DXGI_FORMAT_UNKNOWN;
		lUavResDesc.Buffer.NumElements = ( UINT ) MAX_PARTICLE;
		lUavResDesc.Buffer.StructureByteStride = sizeof(ParticleData);
		lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		lUavResDesc.Buffer.FirstElement = 0;
		lUavResDesc.Buffer.CounterOffsetInBytes = 0;

		device->CreateUnorderedAccessView(particlePool_.Get(),nullptr,&lUavResDesc,cpuHadle);

		particlePoolHandle = gpuHadle;
		gpuHadle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cpuHadle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	//定数バッファ
	CreateConstBufferEmit();
	CreateConstBufferBillboard();
	ConstMapEmit();

	//パイプライン
	CreateFreeListPipeline();
	CreateEmitPipeline();

	//フリーリスト初期化
	{
		cmdList->SetPipelineState(freeListPipState_.Get());
		cmdList->SetComputeRootSignature(freeListRootSig_.Get());

		ID3D12DescriptorHeap* descHeaps[ ] = { descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(descHeaps),descHeaps);

		cmdList->SetComputeRootDescriptorTable(1,freeListUavHandle);
		cmdList->SetComputeRootConstantBufferView(0,constBuffEmit_->GetGPUVirtualAddress());

		cmdList->Dispatch(static_cast< UINT >( emitData.MAX_PARTICLE / 1024 ) + 1,1,1);
	}
}

void GPUParticle::Update(float deltaTime)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	emitData.deltaTime = deltaTime;
	ConstMapEmit();

	//生成
	if ( emitTimeCounter >= timeBetweenEmit )
	{
		emitTimeCounter = 0.0f;

		cmdList->SetPipelineState(emitPipState_.Get());
		cmdList->SetComputeRootSignature(emitRootSig_.Get());

		ID3D12DescriptorHeap* descHeaps[ ] = { descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(descHeaps),descHeaps);

		cmdList->SetComputeRootConstantBufferView(0,constBuffEmit_->GetGPUVirtualAddress());
		cmdList->SetComputeRootDescriptorTable(1,particlePoolHandle);
		cmdList->SetComputeRootDescriptorTable(2,freeListUavHandle);

		cmdList->Dispatch(static_cast< UINT >( emitData.MAX_PARTICLE / 1024 ) + 1,1,1);
	}
	else
	{
		emitTimeCounter += deltaTime;
	}
}

void GPUParticle::Draw()
{
}

void GPUParticle::CreateConstBufferEmit()
{
	HRESULT result;
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = ( sizeof(EmitData) + 0xff ) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffEmit_));
	assert(SUCCEEDED(result));
}

void GPUParticle::CreateConstBufferBillboard()
{
	HRESULT result;
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = ( sizeof(BillboardData) + 0xff ) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffBill_));
	assert(SUCCEEDED(result));
}

void GPUParticle::ConstMapEmit()
{
	HRESULT result;

	// 定数バッファへデータ転送
	EmitData* constMap = nullptr;
	result = constBuffEmit_->Map(0,nullptr,( void** ) &constMap);
	assert(SUCCEEDED(result));
	constMap->pos = emitData.pos;
	constMap->lifeTime = emitData.lifeTime;
	constMap->accel = emitData.accel;
	constMap->scale = emitData.scale;
	constMap->startColor = emitData.startColor;
	constMap->endColor = emitData.endColor;
	constMap->velocity = emitData.velocity;
	constMap->deltaTime = emitData.deltaTime;
	constMap->MAX_PARTICLE = emitData.MAX_PARTICLE;
	constBuffEmit_->Unmap(0,nullptr);
}

void GPUParticle::ConstMapBillboard()
{
	HRESULT result;

	// 定数バッファへデータ転送
	BillboardData* constMap = nullptr;
	result = constBuffEmit_->Map(0,nullptr,( void** ) &constMap);
	assert(SUCCEEDED(result));
	constMap->mat = billboradData.mat;
	constMap->billMat = billboradData.billMat;
	constBuffEmit_->Unmap(0,nullptr);
}

void GPUParticle::CreateFreeListPipeline()
{
	HRESULT result;
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipStateDesc = {};

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> csBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> rootBlob;

	//　頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Particle/FreeListInitCS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main","cs_5_0",	// エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0,&csBlob,&errorBlob
	);

	// シェーダのエラー内容を表示
	if ( FAILED(result) )
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	computePipStateDesc.CS.BytecodeLength = csBlob->GetBufferSize();
	computePipStateDesc.CS.pShaderBytecode = csBlob->GetBufferPointer();

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[ 2 ] = {};
	// ルートパラメータの設定
	rootParams[ 0 ].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
	rootParams[ 0 ].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
	rootParams[ 0 ].Descriptor.RegisterSpace = 0;						// デフォルト値
	rootParams[ 0 ].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	// テクスチャレジスタ0番
	rootParams[ 1 ].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[ 1 ].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[ 1 ].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[ 1 ].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.NumStaticSamplers = 0;

	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootBlob,&errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0,rootBlob->GetBufferPointer(),rootBlob->GetBufferSize(),IID_PPV_ARGS(&freeListRootSig_));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	computePipStateDesc.pRootSignature = freeListRootSig_.Get();

	// パイプラインステートの生成
	result = device->CreateComputePipelineState(&computePipStateDesc,IID_PPV_ARGS(&freeListPipState_));
	assert(SUCCEEDED(result));
}

void GPUParticle::CreateEmitPipeline()
{
	HRESULT result;
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipStateDesc = {};

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> csBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> rootBlob;

	//　頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Particle/EmitCS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main","cs_5_0",	// エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0,&csBlob,&errorBlob
	);

	// シェーダのエラー内容を表示
	if ( FAILED(result) )
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	computePipStateDesc.CS.BytecodeLength = csBlob->GetBufferSize();
	computePipStateDesc.CS.pShaderBytecode = csBlob->GetBufferPointer();

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[ 3 ] = {};
	// ルートパラメータの設定
	rootParams[ 0 ].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
	rootParams[ 0 ].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
	rootParams[ 0 ].Descriptor.RegisterSpace = 0;						// デフォルト値
	rootParams[ 0 ].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

	// デスクリプタレンジの設定(UAV1個目)
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	
	rootParams[ 1 ].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[ 1 ].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[ 1 ].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[ 1 ].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// デスクリプタレンジの設定(UAV2個目)
	D3D12_DESCRIPTOR_RANGE descritorRange2{};
	descritorRange2.NumDescriptors = 1;
	descritorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descritorRange2.BaseShaderRegister = 1;
	descritorRange2.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	
	rootParams[ 2 ].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[ 2 ].DescriptorTable.pDescriptorRanges = &descritorRange2;
	rootParams[ 2 ].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[ 2 ].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.NumStaticSamplers = 0;

	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootBlob,&errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0,rootBlob->GetBufferPointer(),rootBlob->GetBufferSize(),IID_PPV_ARGS(&emitRootSig_));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	computePipStateDesc.pRootSignature = emitRootSig_.Get();

	// パイプラインステートの生成
	result = device->CreateComputePipelineState(&computePipStateDesc,IID_PPV_ARGS(&emitPipState_));
	assert(SUCCEEDED(result));
}

void GPUParticle::CreateParticlePoolBuff()
{
	
}
