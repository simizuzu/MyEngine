#pragma once
#include "Camera.h"
#include <array>
#include <cstdint>
#include <wrl.h>
#include <memory>
#include <string>

class PrimitiveDrawer
{
public:
	//線分の最大数
	static const UINT MaxLineCount = 4096;
	//線分の頂点数
	static const UINT VertexCountLine = 2;
	//線分のインデックス数
	static const UINT IndexCountLine = 0;

	//ブレンドモード
	enum class BlendMode {
		None,		//ブレンドなし
		Normal,		//通常αブレンド
		Add,		//加算
		Subtract,	//減算
		Multily,	//乗算
		Screen,		//スクリーン

		//利用してはいけない
		CountOfBlendMode,
	};

	//頂点データ構造体
	struct VertexPosColor
	{
		MyMath::Vector3 pos;	//xyz座標
		MyMath::Vector4 color;	//RGBA
	};

	//パイプラインセット
	struct PipelineSet
	{
		// ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		// パイプラインステートオブジェクト
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	};

	// メッシュ
	struct Mesh 
	{
		// 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
		// インデックスバッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		// インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView{};
		// 頂点バッファマップ
		VertexPosColor* vertMap = nullptr;
		// インデックスバッファマップ
		uint16_t* indexMap = nullptr;
	};

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static PrimitiveDrawer* GetInstance();

	/// <summary>
	/// リソース生成
	/// </summary>
	/// <param name="size">サイズ</param>
	/// <returns>生成したリソース</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateCommittedResource(UINT64 size);

	/// <summary>
	/// メッシュ生成
	/// </summary>
	/// <param name="vertexCount">頂点数</param>
	/// <param name="indexCount">インデックス数</param>
	/// <returns></returns>
	std::unique_ptr<Mesh> CreateMesh(UINT vertexCount, UINT indexCount);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 3D線分の描画
	/// </summary>
	/// <param name="p1">始点座標</param>
	/// <param name="p2">終点座標</param>
	/// <param name="color">色(RGBA)</param>
	void DrawLine3d(const MyMath::Vector3& p1, const MyMath::Vector3& p2, const MyMath::Vector4& color);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// ビュープロジェクションのセット
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

private:
	PrimitiveDrawer() = default;
	~PrimitiveDrawer() = default;
	PrimitiveDrawer(const PrimitiveDrawer&) = delete;
	PrimitiveDrawer& operator=(const PrimitiveDrawer&) = delete;

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	std::unique_ptr<PipelineSet>
		CreateGraphicsPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, BlendMode blendMode);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	void CreateGraphicsPipelines();

	/// <summary>
	/// 各種メッシュ生成
	/// </summary>
	void CreateMeshes();

	// 線分
	std::unique_ptr<Mesh> line_;
	// 線分の使用インデックス
	uint32_t indexLine_ = 0;
	// 参照するビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;
	// パイプラインセット
	std::array<std::unique_ptr<PipelineSet>, (uint16_t)BlendMode::CountOfBlendMode> pipelineSetLines_;
};

