#pragma once
#include "FbxModel.h"
#include "DirectX12Math.h"
#include "Pipeline.h"
#include "WorldTransform.h"
#include "Camera.h"

#include"FbxLoader.h"

/**
 * @class FbxObject3d
 * @brief FBXモデルを描画するクラス
 */

/// <summary>
/// FBXオブジェクト
/// </summary>
class FbxObject3d
{
public: //静的メンバ関数
	static void StaticInitialize(ID3D12Device* device);

	//3Dオブジェクトのインスタンスを生成
	static FbxObject3d* Create();

	//パイプライン初期化
	static void CrateGrapicsPipeline();

private: //静的メンバ変数
	//ボーンの最大数
	static const int MAX_BONES = 64;

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

	//定数バッファ用データ構造体（スキニング）
	struct ConstBufferDataSkin
	{
		MyMath::Matrix4 bones[MAX_BONES];

		//代入演算子削除
		ConstBufferDataSkin& operator=(const ConstBufferDataSkin&) = delete;

		//コピーコンストラクタ削除
		ConstBufferDataSkin(const ConstBufferDataSkin&) = delete;
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

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

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// 時間を進める
	/// </summary>
	void UpdateTime();

	//setter
	MyMath::Matrix4 GetMatWorld();
	MyMath::Vector3 GetWorldPosition(const MyMath::Vector3& worldPos);

	//コンストラクタ・デストラクタ
	FbxObject3d() = default;
	~FbxObject3d() = default;

protected:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
	//定数バッファ（スキン）
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffSkin;

	//ローカル座標
	MyMath::Vector3 translation_ = { 0.0f,0.0f,0.0f };
	//X,Y,Z軸回りのローカル回転角
	MyMath::Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	//ローカルスケール
	MyMath::Vector3 scale_ = { 1.0f,1.0f,1.0f };

	MyMath::Matrix4 matWorld;
	int8_t pad1[4 ];

	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間（アニメーション）
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
	int8_t pad2[7 ];

	// モデル
	FbxModel* model_ = nullptr;


private:

	//代入演算子削除
	FbxObject3d& operator=(const FbxObject3d&) = delete;
	//コピーコンストラクタ削除
	FbxObject3d(const FbxObject3d&) = delete;
};