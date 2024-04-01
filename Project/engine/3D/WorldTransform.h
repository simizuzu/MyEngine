#pragma once
#include "Camera.h"
#include "FbxModel.h"

/**
 * @class WorldTransform.h
 * @brief トランスフォームを設定するクラス
 */

// 定数バッファ用データ構造体B0
struct ConstBufferDataB0
{
	//Mathematics::Matrix4 mat;	// 3D変換行列
	MyMath::Matrix4 viewproj;
	MyMath::Matrix4 world;
	MyMath::Vector3 cameraPos;

	//代入演算子削除
	ConstBufferDataB0& operator=(const ConstBufferDataB0&) = delete;

	//コピーコンストラクタ削除
	ConstBufferDataB0(const ConstBufferDataB0&) = delete;
};

/// <summary>
/// ワールドトランスフォーム
/// </summary>
class WorldTransform
{
public:
	WorldTransform() = default;
	~WorldTransform() = default;
	//初期化
	void Initialize();
	//更新
	void Update(Camera* camera,bool billboradFlag = false);

	//setter
	void SetTranslation(MyMath::Vector3 translation);
	void SetScale(MyMath::Vector3 scale);
	void SetRotation(MyMath::Vector3 rotation);

	//getter
	MyMath::Vector3 GetTranslation();
	MyMath::Vector3 GetScale();
	MyMath::Vector3 GetRotation();

	/// <summary>
	/// GetGPUVirtualAddress()を返す関数
	/// </summary>
	/// <returns>GetGPUVirtualAddress()</returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress();

private: //メンバ関数
	//定数バッファ生成
	void CreateConstBuffer();

private:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	WorldvViewProCamera constBuffMap;

public:
	//ローカル座標
	MyMath::Vector3 translation_ = { 0.0f,0.0f,0.0f };
	//X,Y,Z軸回りのローカル回転角
	MyMath::Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	//ローカルスケール
	MyMath::Vector3 scale_ = { 1.0f,1.0f,1.0f };

	MyMath::Matrix4 matWorld;

	const WorldTransform* parent = nullptr;

	MyMath::Matrix4* parentMat = nullptr;

private:
};

namespace MyMath {
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <param name="transform">トランスフォーム</param>
	/// <returns>ワールド座標</returns>
	MyMath::Vector3 GetWorldPosition(const WorldTransform& transform);

	MyMath::Vector3 GetWorldPosition(const MyMath::Matrix4& transform);
}