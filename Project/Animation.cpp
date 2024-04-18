#include "Animation.h"
#include "PlayerAppearScene.h"
#include "BossAppearScene.h"
#include "AnimationState.h"

Animation::Animation()
{
	//最初のシーンを呼ぶ
	state = new PlayerAppearScene();
}

Animation::~Animation()
{
	delete state;
}

void Animation::Initialize(Camera* camera)
{
	assert(camera);
	camera_ = camera;
}

void Animation::Update()
{
	//メンバ関数呼び出し
	state->Update(this,camera_);
}

void Animation::Draw()
{
	//メンバ関数呼び出し
	state->Draw(this);
}

void Animation::ChangeState(AnimationState* newState)
{
	delete state;
	state = newState;
}
