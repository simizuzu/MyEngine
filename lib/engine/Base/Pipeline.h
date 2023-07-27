#pragma once
#include <DirectXTex/d3dx12.h>
#include <wrl.h>
#include <array>

struct RootsigSetPip
{
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};

enum class BlendMode
{
	None,	// ブレンド無し
	Alpha,	// アルファ
	Add,	// 加算
	Sub,	// 減算
	Mul,	// 乗算
	Inv,	// 色反転
};

class Pipeline
{
public: // エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数

	/// <summary>
	/// パイプライン呼び出し用関数(スプライト)
	/// </summary>
	static void CreateSpritePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend,ID3D12Device* device, std::array<RootsigSetPip,6>&pipeline);

	/// <summary>
	/// パイプライン呼び出し用関数(Obj3Dオブジェクト)
	/// </summary>
	static void CreateObjPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device, RootsigSetPip& pipeline);

	/// <summary>
	/// パイプライン呼び出し用関数(Fbx3Dオブジェクト)
	/// </summary>
	static void CreateFBXPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3D12Device* device, RootsigSetPip& pipeline);

	/// <summary>
	/// パイプライン呼び出し用関数(パーティクル)
	/// </summary>
	static void CreatePaticlePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3DBlob* gsBlob, ID3D12Device* device, RootsigSetPip& pipeline);

	static void CreatePostEffectPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3D12Device* device, RootsigSetPip& pipeline);

private: // メンバ変数

	// クラス呼び出し
	//DirectXCommon* dxCommon_ = nullptr;
};

