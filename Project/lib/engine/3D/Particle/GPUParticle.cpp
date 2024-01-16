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
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();
	cmdList = DirectXCommon::GetInstance()->GetCommandList();

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

		DrawListUavHandle = gpuHadle;
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
	ConstMapBillboard();

	//パイプライン
	CreateFreeListPipeline();
	CreateEmitPipeline();
	CreateUpdatePipeline();
	CreateDrawPipeline();

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
	cmdList = DirectXCommon::GetInstance()->GetCommandList();

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

		cmdList->Dispatch(static_cast< UINT >( 1000 / 1024 ) + 1,1,1);
	}
	else
	{
		emitTimeCounter += deltaTime;
	}

	//更新
	{
		cmdList->SetPipelineState(updatePipState_.Get());
		cmdList->SetComputeRootSignature(updateRootSig_.Get());

		ID3D12DescriptorHeap* descHeaps[] = { descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(descHeaps),descHeaps);

		cmdList->SetComputeRootConstantBufferView(0, constBuffEmit_->GetGPUVirtualAddress());
		cmdList->SetComputeRootDescriptorTable(1, particlePoolHandle);
		cmdList->SetComputeRootDescriptorTable(2, freeListUavHandle);
		cmdList->SetComputeRootDescriptorTable(3, DrawListUavHandle);

		cmdList->Dispatch(static_cast<UINT>(emitData.MAX_PARTICLE / 1024) + 1, 1, 1);
	}
}

void GPUParticle::Draw(WorldTransform* transform)
{
	billboradData.mat = transform->matWorld;
	billboradData.billMat = transform->matBillboard;

	cmdList = DirectXCommon::GetInstance()->GetCommandList();
	//デスクリプタヒープのセット
	ID3D12DescriptorHeap* descHeaps[ ] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(descHeaps),descHeaps);

	//パイプラインステートの設定
	cmdList->SetPipelineState(drawPipState_.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(drawRootSig_.Get());
	//プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0,GetGpuAddress());
	cmdList->SetGraphicsRootDescriptorTable(1,particlePoolHandle);
	cmdList->SetGraphicsRootDescriptorTable(2,DrawListUavHandle);

	//描画コマンド
	cmdList->DrawInstanced(1,(UINT)emitData.MAX_PARTICLE,0,0);
}

void GPUParticle::CreateConstBufferEmit()
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

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
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

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
	result = constBuffBill_->Map(0,nullptr,( void** ) &constMap);
	assert(SUCCEEDED(result));
	constMap->mat = billboradData.mat;
	constMap->billMat = billboradData.billMat;
	constBuffBill_->Unmap(0,nullptr);
}

void GPUParticle::CreateFreeListPipeline()
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();
	
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
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

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

void GPUParticle::CreateUpdatePipeline()
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipStateDesc = {};

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> csBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> rootBlob;

	//　頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Particle/PUpdateCS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main", "cs_5_0",	// エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0, &csBlob, &errorBlob
	);

	// シェーダのエラー内容を表示
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
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
	D3D12_ROOT_PARAMETER rootParams[4] = {};
	// ルートパラメータの設定
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						// デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

	// デスクリプタレンジの設定(UAV1個目)
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// デスクリプタレンジの設定(UAV2個目)
	D3D12_DESCRIPTOR_RANGE descritorRange2{};
	descritorRange2.NumDescriptors = 1;
	descritorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descritorRange2.BaseShaderRegister = 1;
	descritorRange2.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[2].DescriptorTable.pDescriptorRanges = &descritorRange2;
	rootParams[2].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// デスクリプタレンジの設定(UAV3個目)
	D3D12_DESCRIPTOR_RANGE descritorRange3{};
	descritorRange3.NumDescriptors = 1;
	descritorRange3.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descritorRange3.BaseShaderRegister = 2;
	descritorRange3.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[3].DescriptorTable.pDescriptorRanges = &descritorRange3;
	rootParams[3].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.NumStaticSamplers = 0;

	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&updateRootSig_));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	computePipStateDesc.pRootSignature = updateRootSig_.Get();

	// パイプラインステートの生成
	result = device->CreateComputePipelineState(&computePipStateDesc, IID_PPV_ARGS(&updatePipState_));
	assert(SUCCEEDED(result));
}

void GPUParticle::CreateDrawPipeline()
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> rootBlob;

	//　頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Particle/GPUParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main","vs_5_0",	// エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0,&vsBlob,&errorBlob
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

	//　頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Particle/GPUParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main","ps_5_0",	// エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0,&psBlob,&errorBlob
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

	//　頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Particle/GPUParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main","gs_5_0",	// エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0,&gsBlob,&errorBlob
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

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//デプスの書き込みを禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[ 0 ] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[ 0 ] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//// デスクリプタレンジ
	//CD3DX12_DESCRIPTOR_RANGE descRangeUAV;
	//CD3DX12_DESCRIPTOR_RANGE descRangeUAV2;
	//descRangeUAV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,0); 
	//descRangeUAV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,1);

	//// ルートパラメータの設定
	//CD3DX12_ROOT_PARAMETER rootParams[ 3 ]{};
	//rootParams[ 0 ].InitAsConstantBufferView(0,0,D3D12_SHADER_VISIBILITY_ALL);
	//rootParams[ 1 ].InitAsDescriptorTable(1,&descRangeUAV,D3D12_SHADER_VISIBILITY_ALL);
	//rootParams[ 2 ].InitAsDescriptorTable(1,&descRangeUAV2,D3D12_SHADER_VISIBILITY_ALL);

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

	//// スタティックサンプラー
	//CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//// ルートシグネチャの設定
	//CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	//rootSignatureDesc.Init_1_0(_countof(rootParams),rootParams,0,&samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//// バージョン自動判定のシリアライズ
	//result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootBlob,&errorBlob);
	//// ルートシグネチャの生成
	//result = device->CreateRootSignature(0,rootBlob->GetBufferPointer(),rootBlob->GetBufferSize(),IID_PPV_ARGS(&drawRootSig_));
	//assert(SUCCEEDED(result));

	//gpipeline.pRootSignature = drawRootSig_.Get();

	//// グラフィックスパイプラインの生成
	//result = device->CreateGraphicsPipelineState(&gpipeline,IID_PPV_ARGS(&drawPipState_));
	//assert(SUCCEEDED(result));

	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootBlob,&errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0,rootBlob->GetBufferPointer(),rootBlob->GetBufferSize(),IID_PPV_ARGS(&drawRootSig_));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = drawRootSig_.Get();

	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,IID_PPV_ARGS(&drawPipState_));
	assert(SUCCEEDED(result));
}

D3D12_GPU_VIRTUAL_ADDRESS GPUParticle::GetGpuAddress()
{
	return constBuffBill_->GetGPUVirtualAddress();
}
