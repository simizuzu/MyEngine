#pragma once
#include "AnimationState.h"
#include "DirectX12Math.h"
#include "Sprite.h"
#include "FbxObject3d.h"
#include "FbxLoader.h"
#include "WorldTransform.h"

class PlayerAppearScene : public AnimationState
{
public:
	void Initialize();
	void Update(Animation* pState,Camera* camera) override;
	void Draw(Animation* pState) override;

private:
	void BlackMind();
	void ModelMovie();

private:
	std::unique_ptr<FbxObject3d> robotoObj_;
	WorldTransform robotoTrans;

	std::unique_ptr<Sprite> spriteStageName01_;
	TextureData texStageName01_;

private:
	MyMath::Vector2 blackSize = { 0,100 };
	MyMath::Vector2 fieldNameSize = { 50,300 };
	const float blackTimer_ = 50.0f;
	const float stopBlackSize = 1300.0f;
	float texBlackAlpha = 0.0f;

	uint8_t startCount = 0;
};