#pragma once
#include "SuppressWarning.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <forward_list>
MYENGINE_SUPPRESS_WARNINGS_END

#include "Pipeline.h"

#include "WorldTransform.h"

/**
 * @class ParticleManager.h
 * @brief パーティクルを生成するクラス
 */

/// <summary>
/// パーティクルマネージャ
/// </summary>
class ParticleManager
{
private:
	static const int vertexCount = 1024;	// 頂点数

public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

private:
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// パイプラインをセットする関数
	/// </summary>
	static void CreateCraphicsPipeline();
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform">トランスフォーム</param>
	void Draw(WorldTransform* transform);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="texturename">テクスチャ名</param>
	void LoadTexture(const wchar_t* texturename);

	/// <summary>
	/// バッファ生成
	/// </summary>
	void CreateModel();

	/// <summary>
	/// リストにパーティクルをセットし値を追加していく関数
	/// </summary>
	/// <param name="life">寿命</param>
	/// <param name="position">座標</param>
	/// <param name="velocity">速さ</param>
	/// <param name="accel">加速度</param>
	/// <param name="start_scale">出始めの大きさ</param>
	/// <param name="end_scale">終わるときの大きさ</param>
	/// <param name="s_color">出始めの色</param>
	/// <param name="e_color">終わるときの色</param>
	void Add(int life, MyMath::Vector3 position, MyMath::Vector3 velocity, MyMath::Vector3 accel, float start_scale, float end_scale, MyMath::Vector4 s_color, MyMath::Vector4 e_color);

	/// <summary>
	/// パーティクルをランダムで動かす関数
	/// </summary>
	void RandParticle();

private:
	// デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;
	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_;
	// パイプラインステート
	static RootsigSetPip pip;

	//デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	//シェーダーリソースビュー(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//シェーダーリソースビュー(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//インクリメントサイズ
	UINT descriptorHandleIncrementSize;
	// 頂点データ配列
	VertexPos vertices[vertexCount];
	int8_t pad1[4 ];

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;

	//マットビュー
	MyMath::Matrix4 matView;
	//マットプロジェクション
	MyMath::Matrix4 matProjection;

private:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	//パーティクルの配列
	std::forward_list<Particle> particles;

	//代入演算子削除
	ParticleManager& operator=(const ParticleManager&) = delete;

	//コピーコンストラクタ削除
	ParticleManager(const ParticleManager&) = delete;
};

