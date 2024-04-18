#pragma once
#include "Camera.h"

//クラスの前方宣言
class AnimationState;

class Animation
{
public:
	Animation();
	~Animation();

	void Initialize(Camera* camera);
	void Update();
	void Draw();
	void ChangeState(AnimationState* newState);

public:
	Camera* camera_ = nullptr;
	AnimationState* state;
};