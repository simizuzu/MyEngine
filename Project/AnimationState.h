#pragma once
#include "Camera.h"
#include "Animation.h"

class AnimationState
{
public:
	virtual ~AnimationState() = default;
	virtual void Update(Animation* pState,Camera* camera) = 0;
	virtual void Draw(Animation* pState) = 0;
};
