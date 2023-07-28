#pragma once
#include "FbxModel.h"
#include "DirectX12Math.h"
#include "Pipeline.h"
#include "WorldTransform.h"
#include "Camera.h"

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
	/// 更新
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform">トランスフォーム</param>
	void Draw();

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(FbxModel* model);

protected:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;

	//ローカル座標
	MyMath::Vector3 translation_ = { 0.0f,0.0f,0.0f };
	//X,Y,Z軸回りのローカル回転角
	MyMath::Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	//ローカルスケール
	MyMath::Vector3 scale_ = { 1.0f,1.0f,1.0f };

	MyMath::Matrix4 matWorld;

	// モデル
	FbxModel* model_ = nullptr;
};