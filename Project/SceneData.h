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

	std::unique_ptr<ObjObject3d> skydomeObj_;
	std::unique_ptr<ObjObject3d> buildingObj_;
	std::unique_ptr<ObjObject3d> groundObj_;

	std::unique_ptr<ObjModel> groundModel_;
	std::unique_ptr<ObjModel> skydomeModel_;
	std::unique_ptr<ObjModel> buildingModel_;

	WorldTransform skydomeTrans;
	WorldTransform buildingTrans;
	WorldTransform groundTrans;
};

