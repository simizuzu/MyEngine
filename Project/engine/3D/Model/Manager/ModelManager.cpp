#include "ModelManager.h"

ModelManager* ModelManager::instance = nullptr;

ModelManager* ModelManager::GetInstance()
{
	//インスタンスを生成
	if ( instance == nullptr )
	{
		instance = new ModelManager();
	}

	return instance;
}

void ModelManager::LoadModel(const std::string& filePath,ModelType type,bool smoothing)
{
	//OBJモデル
	if ( type == obj )
	{
		//読み込み済みモデルを検索
		if ( objModels.contains(filePath) )
		{
			//読み込み済みなら早期return
			return;
		}

		std::unique_ptr<ObjModel> objModel;
		objModel.reset(ObjModel::LoadFromObj(filePath,smoothing));

		objModels.insert(std::make_pair(filePath,std::move(objModel)));
	}
	//FBXモデル
	else if ( type == fbx )
	{
		//読み込み済みモデルを検索
		if ( fbxModels.contains(filePath) )
		{
			//読み込み済みなら早期return
			return;
		}

		std::unique_ptr<FbxModel> fbxModel;
		fbxModel.reset(FbxLoader::GetInstance()->LoadModelFromFile(filePath));

		//モデルをmapコンテナに格納する
		fbxModels.insert(std::make_pair(filePath,std::move(fbxModel)));
	}
}

ObjModel* ModelManager::FindObjModel(const std::string& filePath)
{
	//読み込み済みモデルを検索
	if ( objModels.contains(filePath) )
	{
		//読み込みモデルを戻り値としてreturn
		return objModels.at(filePath).get();
	}

	//ファイル名一致なし
	return nullptr;
}

FbxModel* ModelManager::FindFbxModel(const std::string& filePath)
{
	//読み込み済みモデルを検索
	if ( fbxModels.contains(filePath) )
	{
		//読み込みモデルを戻り値としてreturn
		return fbxModels.at(filePath).get();
	}

	//ファイル名一致なし
	return nullptr;
}

void ModelManager::Finalze()
{
	delete instance;
}
