#include "EnemyManager.h"

#include "EnemyNormal.h"
#include "ModelManager.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <fstream>
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

EnemyManager* EnemyManager::Create(Player* player, LevelData* data,const std::string& modelName,Camera* camera)
{
	//インスタンス生成
	EnemyManager* instance = new EnemyManager();
	if ( instance == nullptr )
	{
		return nullptr;
	}

	instance->Initialize(modelName,camera);
	instance->UpdateEnemyPopCommands(player,data);

	return instance;
}

void EnemyManager::Initialize(const std::string& modelName,Camera* camera)
{
	assert(camera);
	camera_ = camera;
	modelName_ = modelName;
}

void EnemyManager::Update()
{
	for ( std::unique_ptr<BaseEnemy>& enemy : enemys )
	{
		enemy->Update();
	}

	DeleteEnemys();
}

void EnemyManager::Draw()
{
	for ( std::unique_ptr<BaseEnemy>& enemy : enemys )
	{
		enemy->Draw();
	}
}

const std::list<std::unique_ptr<BaseEnemy>>& EnemyManager::GetEnemys()
{
	return enemys;
}

MyMath::Vector3 EnemyManager::GetEnemyCenterPos()
{
	return enemysPos_;
}

bool EnemyManager::GetReachCommandFlag()
{
	return reachCommandFlag;
}

void EnemyManager::UpdateEnemyPopCommands(Player* player,LevelData* enemyData)
{
	//jsonファイルの"file_name"から属性分け
	for ( LevelData::EnemyData enemy : enemyData->enemys )
	{
		if ( enemy.enemyType == "normal" )
		{
			std::unique_ptr<EnemyNormal> normalEnemy = std::make_unique<EnemyNormal>();

			normalEnemy->Initialize(modelName_,camera_);
			normalEnemy->SetPlayer(player);
			normalEnemy->SetEnemyTranslation(enemy.translation);

			enemys.push_back(std::move(normalEnemy));
		}

		/*if ( enemy.enemyType == "move" )
		{
			std::unique_ptr<EnemyNormal> normalEnemy = std::make_unique<EnemyNormal>();

			normalEnemy->Initialize(modelName_,camera_);
			normalEnemy->SetPlayer(player);
			normalEnemy->SetEnemyTranslation(enemy.translation);

			enemys.push_back(std::move(normalEnemy));
		}*/

		/*if ( enemy.enemyType == "boss" )
		{
			std::unique_ptr<EnemyNormal> normalEnemy = std::make_unique<EnemyNormal>();

			normalEnemy->Initialize(modelName_,camera_);
			normalEnemy->SetPlayer(player);
			normalEnemy->SetEnemyTranslation(enemy.translation);

			enemys.push_back(std::move(normalEnemy));
		}*/
	}
}

void EnemyManager::DeleteEnemys()
{
	enemys.remove_if([ ] (std::unique_ptr<BaseEnemy>& enemys)
	{
		return enemys->IsDead();
	});
}
