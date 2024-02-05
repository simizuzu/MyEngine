#pragma once
#include "SuppressWarning.h"

#include "ObjModel.h"
#include "FbxLoader.h"
#include "FbxModel.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <map>
#include <string>
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

enum ModelType
{
	obj,
	fbx,
};

class ModelManager
{
public:
	//シングルトンインスタンスの取得
	static ModelManager* GetInstance();

	/// <summary>
	/// モデルファイルの読み込み
	/// </summary>
	/// <param name="filePath">モデルファイルのパス</param>
	void LoadModel(const std::string& filePath, ModelType type, bool smoothing = false);

	/// <summary>
	/// モデルの検索(OBJ)
	/// </summary>
	/// <param name="filePath">モデルのファイルパス</param>
	/// <returns>モデル</returns>
	ObjModel* FindObjModel(const std::string& filePath);

	/// <summary>
	/// モデルの検索(FBX)
	/// </summary>
	/// <param name="filePath">モデルのファイルパス</param>
	/// <returns>モデル</returns>
	FbxModel* FindFbxModel(const std::string& filePath);

	//終了
	void Finalze();

private:
	static ModelManager* instance;

	//モデルデータ(OBJ)
	std::map<std::string, std::unique_ptr<ObjModel>> objModels;
	//モデルデータ(FBX)
	std::map<std::string,std::unique_ptr<FbxModel>> fbxModels;

private:
	//コンストラクタ
	ModelManager() = default;
	//デストラクタ
	~ModelManager() = default;
	//代入演算子削除
	ModelManager& operator=(const ModelManager&) = delete;
	//コピーコンストラクタ削除
	ModelManager(const ModelManager&) = delete;
};

