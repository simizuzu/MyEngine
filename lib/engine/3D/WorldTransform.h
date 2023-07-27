﻿#pragma once
#include "Camera.h"

// 定数バッファ用データ構造体B0
struct ConstBufferDataB0
{
	//Mathematics::Matrix4 mat;	// 3D変換行列
	MyMath::Matrix4 viewproj;
	MyMath::Matrix4 world;
	MyMath::Vector3 cameraPos;
};

class WorldTransform
{

public:
	//初期化
	void Initialize();
	void Update(Camera* camera);
	//更新
	void UpdateParticle(Camera* camera,bool billboradFlag = false);


public:	//セッター・ゲッター
	void SetTranslation(MyMath::Vector3 translation);
	void SetScale(MyMath::Vector3 scale);
	void SetRotation(MyMath::Vector3 rotation);

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
};

