#include "BulletManager.h"

#include "EnemyBullet.h"
#include "PlayerBullet.h"

//インスタンス生成
BulletManager* BulletManager::GetInstance()
{
	static BulletManager instance;
	return &instance;
}

void BulletManager::Initialize(const std::string& modelName,Camera* camera)
{
	assert(camera);

	camera_ = camera;
	modelName_ = modelName;
}

void BulletManager::Update()
{
	for ( std::unique_ptr<BaseBullet>& normalBullet : normalBullets )
	{
		normalBullet->Update(camera_);
	}

	//フラグが立った時削除
	normalBullets.remove_if([ ] (std::unique_ptr<BaseBullet>& normalBullet)
	{
		if ( normalBullet->IsDead() )
		{
			return true;
		}
		return false;
	});
}

void BulletManager::Draw()
{
	for ( std::unique_ptr<BaseBullet>& normalBullet : normalBullets )
	{
		normalBullet->Draw();
	}
}

void BulletManager::CreateNormalBullet(const MyMath::Vector3& pos,const MyMath::Vector3 velocity,Player* player)
{
	std::unique_ptr<EnemyBullet> newBullet;

	newBullet = std::make_unique<EnemyBullet>();
	newBullet->SetPlayer(player);
	newBullet->Initialize(modelName_,pos,velocity);

	normalBullets.push_back(std::move(newBullet));
}

void BulletManager::CreatePlayerBullet(const MyMath::Vector3& pos,const MyMath::Vector3 velocity)
{
	std::unique_ptr<PlayerBullet> newBullet;

	newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(modelName_,pos,velocity);

	normalBullets.push_back(std::move(newBullet));
}

void BulletManager::DeleteBullets()
{
	for ( std::unique_ptr<BaseBullet>& normalBullet : normalBullets )
	{
		delete &normalBullet;
	}
}

const std::list<std::unique_ptr<BaseBullet>>& BulletManager::GetNormalBullets()
{
	return normalBullets;
}
