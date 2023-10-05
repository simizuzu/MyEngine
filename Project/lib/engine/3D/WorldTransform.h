#pragma once
#include "Camera.h"
#include "FbxModel.h"

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

class WorldTransform
{
public:
	WorldTransform() = default;
	~WorldTransform() = default;
	//初期化
	void Initialize();
	//更新
	void Update(Camera* camera);
	void UpdateParticle(Camera* camera,bool billboradFlag = false);

	void SetTranslation(MyMath::Vector3 translation);
	void SetScale(MyMath::Vector3 scale);
	void SetRotation(MyMath::Vector3 rotation);

	MyMath::Vector3 GetTranslation();
	MyMath::Vector3 GetScale();
	MyMath::Vector3 GetRotation();

	void MakeMatWorld();

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

private:
	//代入演算子削除
	WorldTransform& operator=(const WorldTransform&) = delete;
	//コピーコンストラクタ削除
	WorldTransform(const WorldTransform&) = delete;
};

namespace MyMath {
	MyMath::Vector3 GetWorldPosition(WorldTransform& transform);
}