#include "EnemyManager.h"

#include "EnemyNormal.h"
#include "ModelManager.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <fstream>
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

EnemyManager* EnemyManager::Create(Player* player,LevelData* data,const std::string& modelName,Camera* camera)
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

void EnemyManager::Update(Player* player,LevelData* data,const std::string& modelName)
{

	UpdateMoveEnemyPop(player,data,modelName);
	//待機処理
	if ( waitFlag )
	{
		waitTimer--;
		if ( waitTimer <= 0 )
		{
			//待機完了
			waitFlag = false;
		}
	}


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

void EnemyManager::UpdateEnemyPopCommands(Player* player,LevelData* enemyData)
{
	//jsonファイルの"file_name"から属性分け
	for ( LevelData::EnemyData enemy : enemyData->enemys )
	{
		/*if ( enemy.enemyType == "normal" )
		{
			std::unique_ptr<EnemyNormal> normalEnemy = std::make_unique<EnemyNormal>();

			normalEnemy->Initialize(modelName_,camera_);
			normalEnemy->SetPlayer(player);
			normalEnemy->SetEnemyTranslation(enemy.translation);

			enemys.push_back(std::move(normalEnemy));
		}*/

		player = player;

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

void EnemyManager::UpdateMoveEnemyPop(Player* player,LevelData* enemyData,const std::string& modelName)
{

	//jsonファイルの"file_name"から属性分け
	for ( LevelData::EnemyData enemy : enemyData->enemys )
	{
		waitTimer =enemy.waitTime;
		if ( !waitFlag )
		{
			if ( enemy.enemyType == "normal" )
			{
				std::unique_ptr<EnemyNormal> normalEnemy = std::make_unique<EnemyNormal>();

				normalEnemy->Initialize(modelName,camera_);
				normalEnemy->SetPlayer(player);
				normalEnemy->SetEnemyTranslation(enemy.translation);

				enemys.push_back(std::move(normalEnemy));
			}
			break;
		}
		waitFlag = true;
	}
}

void EnemyManager::DeleteEnemys()
{
	enemys.remove_if([ ] (std::unique_ptr<BaseEnemy>& enemys)
	{
		return enemys->IsDead();
	});
}
