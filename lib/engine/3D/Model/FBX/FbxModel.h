#pragma once
#include "DirectX12Math.h"

#pragma warning(push)
#include <string>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#pragma warning(pop)

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
	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;

public: //サブクラス
	struct FBXVertexPosNormalUv
	{
		MyMath::Vector3 pos; //xyz座標
		MyMath::Vector3 normal; //法線ベクトル
		MyMath::Vector2 uv; //uv座標
	};

public:
	//バッファ生成
	void CreateBuffers(ID3D12Device* device);

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	const MyMath::Matrix4& GetModelTransform();

private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;

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
	std::vector<FBXVertexPosNormalUv> vertices;
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
};
