#pragma once
#include "LevelLoader.h"
#include "TextureManager.h"

#include "Sprite.h"

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include "Player.h"

 /**
 * @class SceneData.h
 * @brief SceneDataのクラス
 */

/// <summary>
/// シーンデータ
/// </summary>
class SceneData
{
public:
	
	void Initialize(Camera* camera);
	
	void Update();
	
	void Draw();

	SceneData() = default;
	~SceneData() = default;

private:
	Camera* camera_;
	LevelData* levelData;
	LevelData* curveData2;

	std::unique_ptr<Player> player_;
	
	TextureData displayTex;
	std::unique_ptr<Sprite> display;

	std::unique_ptr<ObjObject3d> skydomeObj_;
	std::unique_ptr<ObjObject3d> buildingObj_;
	std::unique_ptr<ObjObject3d> groundObj_;
	std::unique_ptr<ObjObject3d> tyoinoriObj_;

	std::unique_ptr<ObjModel> groundModel_;
	std::unique_ptr<ObjModel> skydomeModel_;
	std::unique_ptr<ObjModel> buildingModel_;
	std::unique_ptr<ObjModel> tyoinoriModel_;

	WorldTransform tyoinoriTrans;
	WorldTransform skydomeTrans;
	WorldTransform buildingTrans;
	WorldTransform groundTrans;


	float maxTime = 10.0f;
	float timeRate;
	float targetTimeRate;
	int8_t pad1[ 4 ];

	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;
	int8_t pad2[ 4 ];

	std::array<WorldTransform, 100> objs;

private:
	//代入演算子削除
	SceneData& operator=(const SceneData&) = delete;
	//コピーコンストラクタ削除
	SceneData(const SceneData&) = delete;
};

