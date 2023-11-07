#include "EnemyNormal.h"

void EnemyNormal::Initialize(Camera* camera)
{
	//渡されたかチェック
	assert(camera);
	camera_ = camera;

	EnemyNModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("enemyNormal"));
	EnemyNObj_.reset(FbxObject3d::Create());
	EnemyNObj_->SetModel(EnemyNModel_.get());
	EnemyNObj_->PlayAnimation();
	EnemyNObj_->SetScale({ 0.009f,0.009f ,0.009f });
}

void EnemyNormal::Update()
{
	EnemyNObj_->Update(camera_);
}

void EnemyNormal::Draw()
{
	EnemyNObj_->Draw();
}
