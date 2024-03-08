#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <d3dx12.h>
#include <wrl.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class Shader.h
 * @brief シェーダーファイルを読み取るクラス
 */

/// <summary>
/// シェーダー
/// </summary>
class Shader
{
public:
	/// <summary>
	/// スプライトシェーダーファイル読み込み
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	static void CreateSpriteShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob);

	/// <summary>
	/// OBJシェーダーファイル読み込み
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	static void CreateObjShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob);

	/// <summary>
	/// FBXシェーダーファイル読み込み
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	static void CreateFBXShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob);

	/// <summary>
	/// パーティクルシェーダーファイル読み込み
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	/// <param name="psBlob">ジオメトリシェーダブロブ</param>
	static void CreateParticleShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob, Microsoft::WRL::ComPtr<ID3DBlob>& gsBlob);

	/// <summary>
	/// ポストエフェクトシェーダーファイル読み込み
	/// </summary>
	/// <param name="vsBlob">頂点シェーダブロブ</param>
	/// <param name="psBlob">ピクセルシェーダブロブ</param>
	static void CreatePostEffectShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob,Microsoft::WRL::ComPtr<ID3DBlob>& psBlob);
};