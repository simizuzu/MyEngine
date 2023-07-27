#pragma once
#include "DirectXCommon.h"

#include "Vector3.h"
#include "Vector4.h"
#include "DirectX12Math.h"

/// <summary>
/// ライト
/// </summary>
class Light
{
public:
	struct ConstBufferData
	{
		MyMath::Vector4 lightVec;		//ライトへの方向を表すベクトル
		MyMath::Vector3 lightColor;	//ライトの色
	};

public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialise(DirectXCommon* device);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Light* Create();

	static DirectXCommon* device_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList,UINT rootPramIndex);

	/// <summary>
	/// ライト方向をセット
	/// </summary>
	/// <param name="lightdir">ライト方向</param>
	void SetLightDir(const MyMath::Vector4& lightdir);

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="lightColor">ライト色</param>
	void SetLightColor(const MyMath::Vector3& lightColor);

private:
	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

private: //メンバ変数
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	//ライト光線方向（単位ベクトル）
	MyMath::Vector4 lightdir = {1,0,0,0};
	//ライト色
	MyMath::Vector3 lightColor = { 1.0f,1.0f,1.0f };
	//ダーティフラグ
	bool dirty = false;
};

