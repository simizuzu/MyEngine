#pragma once

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <wrl.h>
#include <d3d12.h>
MYENGINE_SUPPRESS_WARNINGS_END

#include "Matrix4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

struct ConstBufferDataViewProjection
{
	// ワールド行列
	MyMath::Matrix4 world;
	// ワールド座標
	MyMath::Matrix4 matWorld;

	// ワールド → ビュー変換行列
	MyMath::Matrix4 view;
	// ビュー → プロジェクション変換行列
	MyMath::Matrix4 projection;
	// カメラ座標（ワールド座標）
	MyMath::Vector3 cameraPos;
};

struct ViewProjection
{
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constBuffMap = nullptr;
};

struct VertexPosNormalUv
{
	MyMath::Vector3 pos;		// xyz座標
	MyMath::Vector3 normal;	// 法線ベクトル
	MyMath::Vector2 uv;		// uv座標
};

struct TextureData
{
	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	//GPUデスクリプタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle{};
	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	// 横幅
	size_t width = 0;
	// 縦幅
	size_t height = 0;
};

//ビルボード定数バッファ用データ構造体
struct ParticleConstBuffData
{
	MyMath::Matrix4 mat;
	MyMath::Matrix4 matBillboard;
};

//カメラ構造体
struct WorldvViewProCamera
{
	//ワールド行列
	MyMath::Matrix4 world;
	//ワールド座標
	MyMath::Matrix4 matWorld;
	//カメラ座標(ワールド座標)
	MyMath::Vector3 cameraPos;
};

struct VertexPos
{
	MyMath::Vector3 pos;	//xyz座標
	float scale;			//スケール
	MyMath::Vector4 color;	//色
};

// パーティクル1粒
struct Particle
{
	// 座標
	MyMath::Vector3 position = {};
	// 速度
	MyMath::Vector3 velocity = {};
	// 加速度
	MyMath::Vector3 accel = {};
	// 現在フレーム
	int frame = 0;
	// 終了フレーム
	int num_frame = 0;
	// スケール
	float scale = 1.0f;
	// 初期値
	float s_scale = 1.0f;
	// 最終値
	float e_scale = 0.0f;
	// 色
	MyMath::Vector4 color = { 1,1,1,1 };
	MyMath::Vector4 s_color = { 1,0,0,1 };
	MyMath::Vector4 e_color = { 1,1,1,1 };
};
