#pragma once
#include "LevelLoader.h"
#include "TextureManager.h"
#include "WorldTransform.h"

#include "Sprite.h"
#include "ObjObject3d.h"
#include "ObjModel.h"

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include "Camera.h"

class SceneData
{
public:
	//初期化
	void Initialize(Camera* camera);
	//更新
	void Update();
	//描画
	void Draw();

private:
	Camera* camera_;
	LevelData* levelData;
	LevelData* curveData2;

	std::unique_ptr<ObjObject3d> skydomeObj_;
	std::unique_ptr<ObjObject3d> buildingObj_;
	std::unique_ptr<ObjObject3d> groundObj_;
	std::unique_ptr<ObjObject3d> tyoinoriObj_;
	std::unique_ptr<ObjObject3d> tyoinoriObj2_;

	std::unique_ptr<ObjModel> groundModel_;
	std::unique_ptr<ObjModel> skydomeModel_;
	std::unique_ptr<ObjModel> buildingModel_;
	std::unique_ptr<ObjModel> tyoinoriModel_;
	std::unique_ptr<ObjModel> tyoinoriModel2_;

	WorldTransform tyoinoriTrans;
	WorldTransform tyoinoriTrans2;
	WorldTransform skydomeTrans;
	WorldTransform buildingTrans;
	WorldTransform groundTrans;


	float maxTime = 10.0f;;				//全体時間[s]
	float timeRate;						//何％時間が進んだか
	float targetTimeRate;
	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;

	std::array<WorldTransform, 10000> objs;
};

