#pragma once
#include "DirectX12Math.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <string>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class FbxModel.h
 * @brief ローダーからバッファなどを生成するクラス
 */

/// <summary>
/// ノード
/// </summary>
struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	MyMath::Vector3 scaling = { 1.0f,1.0f,1.0f };
	//ローカル回転角
	MyMath::Vector3 rotation = { 1.0f,1.0f,1.0f };
	//ローカル移動
	MyMath::Vector3 translation = { 0.0f,0.0f,0.0f };
	//ローカル変形行列
	MyMath::Matrix4 transform;
	//グローバル変形行列
	MyMath::Matrix4 globalTransform;
	int8_t pad1[ 4 ];
	//親ノード
	Node* parent = nullptr;
	int16_t pad2[ 4 ];

	Node() = default;
};

/// <summary>
/// FBXモデル
/// </summary>
class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;

public: //定数
	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;

public: //サブクラス
	struct VertexPosNormalUvSkin
	{
		MyMath::Vector3 pos; //xyz座標
		MyMath::Vector3 normal; //法線ベクトル
		MyMath::Vector2 uv; //uv座標
		UINT boneIndex[ MAX_BONE_INDICES ];	//ボーン番号
		float boneWeight[ MAX_BONE_INDICES ];	//ボーン重み
	};

public:
	/// <summary>
	/// ボーン
	/// </summary>
	struct Bone
	{
		//名前
		std::string name_;
		//初期姿勢の逆行列
		MyMath::Matrix4 invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		//コンストラクタ
		Bone(const std::string& name);
		//代入演算子削除
		Bone& operator=(const Bone&) = delete;
	};

public:
	//バッファ生成
	void CreateBuffers(ID3D12Device* device);

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	FbxModel() = default;
	~FbxModel();

	//getter
	std::vector<Bone>& GetBones();
	FbxScene* GetFbxScene();

	const MyMath::Matrix4& GetModelTransform();

private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
	//ボーン配列
	std::vector<Bone> bones;

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRVヒープ用デスクリプタビープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	//頂点インデックス配列
	std::vector<unsigned short> indices;

	//アンビエント係数
	MyMath::Vector3 ambient = { 1,1,1 };
	//ディフーズ係数
	MyMath::Vector3 diffuse = { 1,1,1 };
	//テクスチャメタデータ
	DirectX::TexMetadata metadata = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};

	//FBXシーン
	FbxScene* fbxScene = nullptr;

private:
	FbxModel& operator=(const FbxModel&) = delete;
	FbxModel(const FbxModel&) = delete;
};
