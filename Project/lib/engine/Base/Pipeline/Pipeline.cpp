#include "Pipeline.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include<cassert>
MYENGINE_SUPPRESS_WARNINGS_END

void Pipeline::CreateSpritePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device, std::array<RootsigSetPip, 6>& pipeline)
{
	HRESULT result;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	// エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob;

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz座標
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// 頂点シェーダ、ピクセルシェーダをパイプラインに設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// カリングしない
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// 前面カリング
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// 背面カリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// 深度クリッピングを有効に
	// ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0];
	// ブレンドデスク
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0]; // RGBA全てのチャンネルを描画
	// 共通設定
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	switch (blend)
	{
	case BlendMode::None: // ブレンド無し
		blenddesc.BlendEnable = false;
		break;

	case BlendMode::Alpha: // アルファ
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case BlendMode::Add: // 加算
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::Sub: // 減算
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMode::Mul: // 乗算
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ZERO;
		blenddesc.DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case BlendMode::Inv: // 色反転
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_COLOR;;
		blenddesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	}

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 三角形
	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// ルートパラメータの設定
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						// デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	// テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//定数バッファ1番
	rootParams[ 2 ].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[ 2 ].Descriptor.ShaderRegister = 1;					//デスクリプタレンジ
	rootParams[ 2 ].Descriptor.RegisterSpace = 0;					//デスクリプタレンジ数
	rootParams[ 2 ].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見えるバッファ

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 横繰り返し(タイリング)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 縦繰り返し(タイリング)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 奥行繰り返し(タイリング)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// ボーダーの時は黒
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// 全てリニア補間
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;									// ミップマップ最大値
	sampleDesc.MinLOD = 0.0f;												// ミップマップ最小値
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// ピクセルシェーダ殻のみ使用可能

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline[static_cast<size_t>(blend)].rootSignature));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = pipeline[static_cast<size_t>(blend)].rootSignature.Get();

	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline[static_cast<size_t>(blend)].pipelineState));
	assert(SUCCEEDED(result));
}

void Pipeline::CreateObjPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device,RootsigSetPip& pipeline)
{
	HRESULT result;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	// エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob;

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz座標
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // 法線ベクトル
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// 頂点シェーダ、ピクセルシェーダをパイプラインに設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザの設定
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// カリングしない
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// 前面カリング
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// 背面カリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// 深度クリッピングを有効に
	// ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0];
	// ブレンドデスク
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0]; // RGBA全てのチャンネルを描画
	// 共通設定
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	// あまり使わなそうなので今は半透明のみ
	if (blend == BlendMode::Alpha)
	{
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	}
	else
	{
		blenddesc.BlendEnable = false;
	}

	if (blend == BlendMode::None)
	{
		blenddesc.BlendEnable = false;
	}
	else
	{
		blenddesc.BlendEnable = false;
	}

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 三角形
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度フォーマット
	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descritorRange{};
	descritorRange.NumDescriptors = 1;
	descritorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descritorRange.BaseShaderRegister = 0;
	descritorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[4] = {};
	// 定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						// デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

	// テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descritorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[2].Descriptor.ShaderRegister = 1;					//デスクリプタレンジ
	rootParams[2].Descriptor.RegisterSpace = 0;						//デスクリプタレンジ数
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見えるバッファE

	//定数バッファ1番
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[3].Descriptor.ShaderRegister = 2;					//デスクリプタレンジ
	rootParams[3].Descriptor.RegisterSpace = 0;						//デスクリプタレンジ数
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見えるバッファE

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 横繰り返し(タイリング)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 縦繰り返し(タイリング)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// 奥行繰り返し(タイリング)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// ボーダーの時は黒
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// 全てリニア補間
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;									// ミップマップ最大値
	sampleDesc.MinLOD = 0.0f;												// ミップマップ最小値
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// ピクセルシェーダ殻のみ使用可能

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline.rootSignature));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = pipeline.rootSignature.Get();

	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline.pipelineState));
	assert(SUCCEEDED(result));
}

void Pipeline::CreateFBXPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3D12Device* device, RootsigSetPip& pipeline)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 影響を受けるボーン番号(4つ)
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // ボーンのスキンウェイト(4つ)
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA(SRGB版)
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	// CBV（スキニング用）
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline.rootSignature));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = pipeline.rootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline.pipelineState));
	if (FAILED(result)) { assert(0); }
}

void Pipeline::CreatePaticlePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3DBlob* gsBlob, ID3D12Device* device, RootsigSetPip& pipeline)
{
	HRESULT result;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	// エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob;

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ //xyz座標
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ //uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ //色
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
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
	////加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;
	//減算合成
	/*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/

	//blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	//blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//デプスの書き込みを禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.AlphaToCoverageEnable = true;
	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2]{};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline.rootSignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = pipeline.rootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline.pipelineState));
	assert(SUCCEEDED(result));
}

void Pipeline::CreatePostEffectPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3D12Device* device, RootsigSetPip& pipeline)
{
	HRESULT result;

	ComPtr<ID3DBlob> errorBlob;		//エラーオブジェクト

	//頂点レイアウト配列の宣言と設定
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	//xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{	//uv座標
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	//パイプラインステートの設定変数の宣言と、各種項目の設定
	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);

	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);	//一旦標準値をセット
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;			//背面カリングをしない

	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	//一旦標準値をセット
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;		//常に上書きルール

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//環境設定

	//共通設定
	blenddesc.BlendEnable = true;	//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソ

	//深度値フォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline.NumRenderTargets = 1;	//描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1;	//1ピクセルにつき1回サンプリング

	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 レジスタ

	//ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	//スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0,D3D12_FILTER_MIN_MAG_MIP_POINT);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//ルートシグネチャの生成
	ComPtr<ID3DBlob> rootSigBlob;

	//バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob,
		&errorBlob);
	assert(SUCCEEDED(result));

	//ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipeline.rootSignature));
	assert(SUCCEEDED(result));

	//グラフィックスパイプラインステートの生成
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = pipeline.rootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline.pipelineState));
	assert(SUCCEEDED(result));
}
