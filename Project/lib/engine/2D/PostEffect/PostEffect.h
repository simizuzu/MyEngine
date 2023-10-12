#pragma once
#include "DirectX12Math.h"

#include "Pipeline.h"
#include "Shader.h"

/**
 * @class PostEffect.h
 * @brief ポストエフェクトを作成するクラス
 */

//定数バッファ用データ（マテリアル）
struct ConstBufferDataPE
{
	MyMath::Vector4	color;//色（RGB）
	MyMath::Matrix4	mat;//3D変換行列

	//コピーコンストラクタ削除
	ConstBufferDataPE& operator=(const ConstBufferDataPE&) = delete;
	//代入演算子削除
	ConstBufferDataPE(const ConstBufferDataPE&) = delete;
};

/// <summary>
/// ポストエフェクト
/// </summary>
class PostEffect
{
public: // 静的メンバ関数

	/// <summary>
	/// パイプラインステート静的初期化
	/// </summary>
	void CreateGraphicsPipeline();

private: // 静的メンバ変数
	//デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_;

	static const float clearColor[4];
	static RootsigSetPip pipline_;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList_);

	/// <summary>
	/// シーン描画前
	/// </summary>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList_);

	/// <summary>
	/// シーン描画後
	/// </summary>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList_);

private:
	/// <summary>
	/// テクスチャ生成
	/// </summary>
	void CreateTex();

	/// <summary>
	/// SRV生成
	/// </summary>
	void CreateSRV();

	/// <summary>
	/// RTV生成
	/// </summary>
	void CreateRTV();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepth();

	/// <summary>
	/// DSV生成
	/// </summary>
	void CreateDSV();

	/// <summary>
	/// 頂点バッファ生成
	/// </summary>
	void CreateVertexBuffer();

private://構造体
	//頂点データ
	struct Vertex
	{
		MyMath::Vector3	pos;//xyz座標
		MyMath::Vector2	uv;//uv座標
	};

	enum VertNum
	{
		LB,//左下
		LT,//左上
		RB,//右下
		RT,//右上
	};

private:
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	//定数バッファビュー
	ConstBufferDataPE* constMap;
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>texBuff;
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapSRV;
	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;
	//RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;
	Vertex vertices_[4];
	//カラー
	MyMath::Vector4 color = { 1,1,1,1 };
	HRESULT result_;
	char pad[4];
};