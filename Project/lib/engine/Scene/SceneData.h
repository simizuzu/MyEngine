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

	std::unique_ptr<ObjObject3d> planeObj_;
	std::unique_ptr<ObjModel> planeModel_;

	WorldTransform planeTrans;

	float rot = 0.0f;
};

