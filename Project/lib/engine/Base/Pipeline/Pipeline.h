#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <d3dx12.h>
#include <wrl.h>
#include <array>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class Pipeline.h
 * @brief 各パイプラインをまとめたクラス
 */

/// <summary>
/// ルートシグネチャとパイプラインをまとめた構造体
/// </summary>
struct RootsigSetPip
{
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};

/// <summary>
/// ブレンドモード
/// </summary>
enum class BlendMode
{
	None,	// ブレンド無し
	Alpha,	// アルファ
	Add,	// 加算
	Sub,	// 減算
	Mul,	// 乗算
	Inv,	// 色反転
};

/// <summary>
/// パイプライン
/// </summary>
class Pipeline
{
public: // エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数

	/// <summary>
	/// パイプライン呼び出し用関数(スプライト)
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	/// <param name="blend">ブレンド値</param>
	/// <param name="device">デバイス</param>
	/// <param name="pipeline">ルートシグネチャとパイプラインをまとめた構造体</param>
	static void CreateSpritePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend,ID3D12Device* device, std::array<RootsigSetPip,6>&pipeline);

	/// <summary>
	/// パイプライン呼び出し用関数(Obj3Dオブジェクト)
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	/// <param name="blend">ブレンド値</param>
	/// <param name="device">デバイス</param>
	/// <param name="pipeline">ルートシグネチャとパイプラインをまとめた構造体</param>
	static void CreateObjPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device, RootsigSetPip& pipeline);

	/// <summary>
	/// パイプライン呼び出し用関数(Fbx3Dオブジェクト)
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	/// <param name="device">デバイス</param>
	/// <param name="pipeline">ルートシグネチャとパイプラインをまとめた構造体</param>
	static void CreateFBXPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3D12Device* device, RootsigSetPip& pipeline);

	/// <summary>
	/// パイプライン呼び出し用関数(パーティクル)
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	/// <param name="gsBlob">ジオメトリシェーダブロブ</param>
	/// <param name="device">デバイス</param>
	/// <param name="pipeline">ルートシグネチャとパイプラインをまとめた構造体</param>
	static void CreatePaticlePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3DBlob* gsBlob, ID3D12Device* device, RootsigSetPip& pipeline);

	/// <summary>
	/// パイプライン呼び出し用関数(ポストエフェクト)
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	/// <param name="device">デバイス</param>
	/// <param name="pipeline">ルートシグネチャとパイプラインをまとめた構造体</param>
	static void CreatePostEffectPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3D12Device* device, RootsigSetPip& pipeline);
};

