#pragma once
#include "LevelLoader.h"
#include "TextureManager.h"

#include "Sprite.h"

#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxObject3d.h"

#include "Player.h"

class SceneData
{
public:
	
	void Initialize(Camera* camera);
	
	void Update();
	
	void Draw();

private:
	Camera* camera_;
	LevelData* levelData;

	Player* player = nullptr;
	

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


	float maxTime = 10.0f;;
	float timeRate;
	float targetTimeRate;
	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;

	std::array<WorldTransform, 10000> objs;
};

