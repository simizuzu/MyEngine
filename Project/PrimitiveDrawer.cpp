#include "PrimitiveDrawer.h"
#include "DirectXCommon.h"
#include <cassert>
#include <d3dcompiler.h>
#include <d3dx12.h>

#pragma comment(lib, "d3dcompiler.lib")

PrimitiveDrawer* PrimitiveDrawer::GetInstance()
{
    static PrimitiveDrawer instance;
    return &instance;;
}

Microsoft::WRL::ComPtr<ID3D12Resource> PrimitiveDrawer::CreateCommittedResource(UINT64 size)
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

	// リソース生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));

	return resource;
}

std::unique_ptr<PrimitiveDrawer::Mesh> PrimitiveDrawer::CreateMesh(UINT vertexCount, UINT indexCount)
{
	HRESULT result;
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();

	if (vertexCount > 0) {
		// 頂点データのサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(VertexPosColor) * vertexCount);
		// 頂点バッファ生成
		mesh->vertBuff = CreateCommittedResource(sizeVB);

		// 頂点バッファビューの作成
		mesh->vbView.BufferLocation = mesh->vertBuff->GetGPUVirtualAddress();
		mesh->vbView.SizeInBytes = sizeVB;
		mesh->vbView.StrideInBytes = sizeof(VertexPosColor);

		// 頂点バッファのマッピング
		result = mesh->vertBuff->Map(0, nullptr, reinterpret_cast<void**>(&mesh->vertMap));
		assert(SUCCEEDED(result));
	}

	if (indexCount > 0) {
		// インデックスデータのサイズ
		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexCount);
		// インデックスバッファ生成
		mesh->indexBuff = CreateCommittedResource(sizeIB);

		// インデックスバッファビューの作成
		mesh->ibView.BufferLocation = mesh->indexBuff->GetGPUVirtualAddress();
		mesh->ibView.Format = DXGI_FORMAT_R16_UINT;
		mesh->ibView.SizeInBytes = sizeIB;

		// インデックスバッファのマッピング
		result = mesh->indexBuff->Map(0, nullptr, reinterpret_cast<void**>(&mesh->indexMap));
		assert(SUCCEEDED(result));
	}

	return mesh;
}

void PrimitiveDrawer::Initialize()
{
	CreateGraphicsPipelines();
	CreateMeshes();
}

void PrimitiveDrawer::DrawLine3d(const MyMath::Vector3& p1, const MyMath::Vector3& p2, const MyMath::Vector4& color)
{
	assert(indexLine_ < MaxLineCount);

	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	std::vector<VertexPosColor> vertices;

	// 頂点データ
	vertices = {
	  {p1, color},
	  {p2, color},
	};

	size_t indexVertex = indexLine_ * VertexCountLine;

	assert(vertices.size() <= VertexCountLine);
	// 頂点バッファへのデータ転送
	std::copy(vertices.begin(), vertices.end(), &line_->vertMap[indexVertex]);

	// パイプラインステートの設定
	commandList->SetPipelineState(pipelineSetLines_[(uint16_t)blendMode_]->pipelineState.Get());
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(
		pipelineSetLines_[(uint16_t)blendMode_]->rootSignature.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &line_->vbView);
	//  CBVをセット（ビュープロジェクション行列）
	commandList->SetGraphicsRootConstantBufferView(0, viewProjection_->constBuff_->GetGPUVirtualAddress());
	// 描画コマンド
	commandList->DrawInstanced(VertexCountLine, 1, static_cast<INT>(indexVertex), 0);
	// 使用カウント上昇
	indexLine_++;
}

void PrimitiveDrawer::Reset() { indexLine_ = 0; }

std::unique_ptr<PrimitiveDrawer::PipelineSet> PrimitiveDrawer::CreateGraphicsPipeline(
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, BlendMode blendMode)
{
	std::unique_ptr<PipelineSet> pipelineSet = std::make_unique<PipelineSet>();

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;    // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト
	HRESULT result;

	UINT flag1 = 0;
#ifdef _DEBUG
	// デバッグ可能にする
	flag1 |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PrimitiveVS.hlsl", // シェーダファイル名
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", flag1, 0, &vsBlob, &errorBlob);
	assert(SUCCEEDED(result));

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PrimitivePS.hlsl", // シェーダファイル名
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", flag1, 0, &psBlob, &errorBlob);
	assert(SUCCEEDED(result));

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	  {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// カリングしない
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//  デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (blendMode) {
	case BlendMode::None:
		blenddesc.BlendEnable = false;
		break;
	case BlendMode::Normal:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	case BlendMode::Add:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::Subtract:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::Multily:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ZERO;
		blenddesc.DestBlend = D3D12_BLEND_SRC_COLOR;
		break;
	case BlendMode::Screen:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	default:
		break;
	}

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	gpipeline.PrimitiveTopologyType = topologyType;

	gpipeline.NumRenderTargets = 1;                            // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[1] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet->rootSignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = pipelineSet->rootSignature.Get();

	// グラフィックスパイプラインの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&pipelineSet->pipelineState));
	assert(SUCCEEDED(result));

	return pipelineSet;
}

void PrimitiveDrawer::CreateGraphicsPipelines()
{
	pipelineSetLines_[(uint16_t)BlendMode::None] = 
		CreateGraphicsPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::None);
	pipelineSetLines_[(uint16_t)BlendMode::Normal] =
		CreateGraphicsPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::Normal);
	pipelineSetLines_[(uint16_t)BlendMode::Add] =
		CreateGraphicsPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::Add);
	pipelineSetLines_[(uint16_t)BlendMode::Subtract] =
		CreateGraphicsPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::Subtract);
	pipelineSetLines_[(uint16_t)BlendMode::Multily] =
		CreateGraphicsPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::Multily);
	pipelineSetLines_[(uint16_t)BlendMode::Screen] =
		CreateGraphicsPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::Screen);
}

void PrimitiveDrawer::CreateMeshes()
{
	// 線分メッシュ生成
	UINT lineVertexCount = MaxLineCount * VertexCountLine;
	UINT lineIndexCount = MaxLineCount * IndexCountLine;

	line_ = CreateMesh(lineVertexCount, lineIndexCount);
}
