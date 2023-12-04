#include "EnemyNormal.h"

void EnemyNormal::Initialize(FbxModel* model,Camera* camera)
{
	//渡されたかチェック
	assert(camera);
	assert(model);
	camera_ = camera;
	EnemyModel_ = model;

	EnemyObj_.reset(FbxObject3d::Create());
	EnemyObj_->SetModel(EnemyModel_);
	EnemyObj_->PlayAnimation();
	EnemyObj_->SetScale({ 0.009f,0.009f ,0.009f });
}

void EnemyNormal::Update()
{
	EnemyObj_->SetTranslation(translation);
	EnemyObj_->Update(camera_);
}

void EnemyNormal::Draw()
{
	EnemyObj_->Draw();
}
