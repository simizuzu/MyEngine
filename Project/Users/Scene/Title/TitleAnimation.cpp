#include "TitleAnimation.h"
#include "FbxLoader.h"
#include "Numbers.h"

#include "ModelManager.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file TitleAnimation.cpp
 * @brief Titleの動きに関する処理が書かれてあります
 */

void TitleAnimation::Initalize(Camera* camera)
{
	camera_ = camera;

#pragma region Obj
	skydomeObj_.reset(ObjObject3d::Create());
	skydomeObj_->SetModel("skydome",true);
	skydomeTrans.Initialize();

	skydomeTrans.SetScale({ skydomeScale,skydomeScale,skydomeScale });
#pragma endregion
}

void TitleAnimation::Update()
{
	skydomeTrans.Update(camera_);
}

void TitleAnimation::Draw()
{
	skydomeObj_->Draw(&skydomeTrans);
}

void TitleAnimation::Finalize()
{
	
}

