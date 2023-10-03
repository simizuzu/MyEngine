#include "DebugCamera.h"

void DebugCamera::Initialize(Input* input, Camera* camera)
{
	camera_ = camera;
	input_ = input;

	assert(camera_);
	assert(input_);
	
	matrot = MyMathUtility::MakeRotation(rotation_);
}

void DebugCamera::Update()
{
}
