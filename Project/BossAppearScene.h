#pragma once
#include "AnimationState.h"

class BossAppearScene : public AnimationState
{
public:
	void Initialize();
	void Update(Animation* pState,Camera* camera) override;
	void Draw(Animation* pState) override;
};
