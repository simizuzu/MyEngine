#pragma once
#include "FbxModel.h"
#include "DirectX12Math.h"
#include "Pipeline.h"
#include "WorldTransform.h"

class FbxObject3d
{
public: //静的メンバ関数
	static void StaticInitialize(ID3D12Device* device);

	static FbxObject3d* Create();

	//パイプライン初期化
	static void CrateGrapicsPipeline();

private: //静的メンバ変数
	//デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;

	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_;

	// パイプラインステート
	static RootsigSetPip pip;

public:
	struct ConstBufferDataTransformFbx
	{
		MyMath::Matrix4 viewproj;
		MyMath::Matrix4 world;
		MyMath::Vector3 cameraPos;
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform">トランスフォーム</param>
	void Draw(WorldTransform* transform);

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(FbxModel* model);

protected:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;

	// モデル
	FbxModel* model_ = nullptr;
};