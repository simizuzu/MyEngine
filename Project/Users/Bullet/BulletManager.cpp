#include "BulletManager.h"

#include "Player.h"
#include "EnemyBullet.h"

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
	//フラグが立った時削除
	/*normalBullets.remove_if([ ] (std::unique_ptr<BaseBullet>& normalBullet)
	{
		if ( normalBullet->IsDead() )
		{
			delete& normalBullet;
			return true;
		}
		return false;
	});*/

	for ( std::unique_ptr<BaseBullet>& normalBullet : normalBullets )
	{
		normalBullet->Update(camera_);
	}
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
