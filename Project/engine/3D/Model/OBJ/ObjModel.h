#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <string>
#include <wrl.h>
#include <unordered_map>
#include <map>
MYENGINE_SUPPRESS_WARNINGS_END

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

#include "TextureManager.h"
#include "DirectXCommon.h"
#include "EngineUtility.h"

/**
 * @class ObjLoader.h
 * @brief ファイルからOBJモデルを読み込むクラス
 */

/// <summary>
/// マテリアル
/// </summary>
struct Material
{
	std::string name;				// マテリアル名
	MyMath::Vector3 ambient;	// アンビエント影響度
	MyMath::Vector3 diffuse;	// ディフューズ影響度
	MyMath::Vector3 specular;	// スペキュラー影響度
	float alpha;					// アルファ
	std::string textureFilename;	// テクスチャファイル名
	float shininess = 1.0f;
	int8_t pad[ 4 ];

	// コンストラクタ
	Material();
};

/// <summary>
/// 3Dモデル
/// </summary>
class ObjModel
{
public:
	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		MyMath::Vector3 ambient;	// アンビエント係数
		float pad1;						// パディング
		MyMath::Vector3 diffuse;	// ディフューズ係数
		float pad2;						// パディング
		MyMath::Vector3 specular;	// スペキュラー係数
		float alpha;					// アルファ
	};

public:
	/// <summary>
	/// OBJファイルから3Dモデルを読み込む
	/// </summary>
	/// <returns>モデル</returns>
	static ObjModel* LoadFromObj(const std::string& modelname,bool smoothing = false);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath,const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	void LoadTexture(const std::string& directoryPath,const std::string& filename);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="rootParamIndexMaterial">マテリアル用ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	static void SetDevice(ID3D12Device* device);

	static void Finalize();

private:
	static ID3D12Device* device_;
	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
   // シェーダリソースビューのハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// シェーダリソースビューのハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// デスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
   // インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
   // 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
   // 頂点インデックス配列
	std::vector<unsigned short> indices;
   // マテリアル
	Material material;

private: // メンバ変数
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	int8_t pad1[ 4 ];
	// 定数バッファ（マテリアル）
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource>indexBuff;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	int8_t pad2[ 4 ];
	// テクスチャデータ
	TextureData textureData;

	//頂点法線スムージング用データ
	std::unordered_map<unsigned short,std::vector<unsigned short>> smoothData;

	//ファイル名
	std::string filename_;

private:
	/// <summary>
	/// OBJファイルから3Dモデルを読み込む
	/// </summary>
	void LoadFromOBJInternal(const std::string& modelname,bool smoothing);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// 各種バッファ生成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// 頂点データ全体のサイズ
	/// </summary>
	void CreateVBSize();

	/// <summary>
	/// インデックスデータ全体のサイズ
	/// </summary>
	void CreateIBSize();

	/// <summary>
	/// 頂点データの数を取得
	/// </summary>
	/// <returns>頂点データの取得</returns>
	inline size_t GetVertexCount();

	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	/// <param name="indexPosition">座標インデックス</param>
	/// <param name="indexVertex">頂点インデックス</param>
	void AddSmoothData(unsigned short indexPosition,unsigned short indexVertex);

	/// <summary>
	/// 平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();

public:
	//頂点データを取得
	virtual const std::vector<VertexPosNormalUv> GetVertices();

	//コピーコンストラクタを削除・代入演算子削除
	ObjModel() = default;
	virtual ~ObjModel() = default;
private:
	ObjModel& operator=(const ObjModel&) = delete;
	ObjModel(const ObjModel&) = delete;
};

