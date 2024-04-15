#pragma once
#include "SuppressWarning.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include "fbxsdk.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
MYENGINE_SUPPRESS_WARNINGS_END

#include "FbxModel.h"

/**
 * @class FbxLoader.h
 * @brief ファイルからFBXモデルを読み込むクラス
 */

/// <summary>
/// FBXローダー
/// </summary>
class FbxLoader
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	FbxModel* LoadModelFromFile(const std::string& modelName);

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// FBXの行列をMatrix4に変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(MyMath::Matrix4* dst, const FbxMatrix& src);

private:
	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* model, FbxNode* fbxNode);

	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

	//--------- サブ関数 ---------//
	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);
	//面情報読み取り
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FbxModel* model, FbxNode* fbxNode);
	//スキニング情報の読み取り
	void ParseSkin(FbxModel* model, FbxMesh* fbxMesh);

	//テクスチャ読み込み
	void LoadTexture(FbxModel* model, const std::string& fullpath);

private:
	//同一頂点の法線座標かUVが重なる際の関数(新しく頂点インデックス作成)
	static bool IsExistNormalUVInfo(const std::vector<float>& vertexInfo);

	// 頂点情報生成
	static std::vector<float> CreateVertexInfo(const std::vector<float>& vertex,const FbxVector4& normalVec4,const FbxVector2& uvVec2);

	// 新しく頂点index生成
	static int CreateNewVertexIndex(const std::vector<float>& vertexInfo,const FbxVector4& normalVec4,const FbxVector2& uvVec2,
		std::vector<std::vector<float>>& vertexInfoList,int oldIndex,std::vector<std::array<int,2>>& oldNewIndexPairList);

	// 一括にまとめたpos,norm,uv情報を整理
	static bool IsSetNormalUV(const std::vector<float> vertexInfo,const FbxVector4& normalVec4,const FbxVector2& uvVec2);

	void SetBoneDataToVertices(FbxMesh* pMesh, FbxModel* pModel, std::vector<FbxModel::VertexPosNormalUvSkin>& vertices);

	int FindJointIndexByName(const std::string& name,FbxModel* model);

	std::vector<std::vector<int>> meshVertice;
	std::unordered_map<int,std::vector<int>> meshVerticeControlpoints;

	template<typename T>
	T Min(T a,T b) {
		return ( a < b ) ? a : b;
	}

private:
	//D3Dデバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;

	static FbxLoader* instance;

public:
	static const std::string baseDirectory;

	static const std::string defaultTextureFileName;

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader();
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;
};