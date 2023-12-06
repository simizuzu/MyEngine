#include "EnemyManager.h"

#include "EnemyNormal.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <fstream>
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

EnemyManager* EnemyManager::Create(const std::string& filePath,FbxModel* model,Camera* camera)
{
	//インスタンス生成
	EnemyManager* instance = new EnemyManager();
	if ( instance == nullptr )
	{
		return nullptr;
	}

	instance->Initialize(model,camera);
	instance->LoadEnemyPopData(filePath);

	return instance;
}

void EnemyManager::Initialize(FbxModel* model,Camera* camera)
{
	assert(camera);
	assert(model);
	camera_ = camera;
	model_ = model;
}

void EnemyManager::Update(Player* player)
{
	//敵出現
	UpdateEnemyPopCommands(player);

	for ( std::unique_ptr<BaseEnemy>& enemy : enemys )
	{
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for ( std::unique_ptr<BaseEnemy>& enemy : enemys )
	{
		enemy->Draw();
	}
}

void EnemyManager::EnemyNormalEmit()
{
	//std::unique_ptr<BaseEnemy> enemy = std::make_unique<EnemyNormal>();

	//enemys.push_back(std::move(enemy));
}

void EnemyManager::LoadEnemyPopData(const std::string& filePath)
{
	//ファイルを開く
	std::ifstream file;

	file.open(filePath);
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPospCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void EnemyManager::UpdateEnemyPopCommands(Player* player)
{
	//待機処理
	if ( waitFlag )
	{
		waitTimer--;
		if ( waitTimer <= 0 )
		{
			//待機完了
			waitFlag = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while ( getline(enemyPospCommands,line) )
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//[,]区切りで行の先頭文字列を取得
		getline(line_stream,word,',');

		//["//"]から始まる行はコメントアウト扱い
		if ( word.find("//") == 0 )
		{
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if ( word.find("POP") == 0 )
		{
			
			//size_t enemyType = static_cast< size_t >( std::atoi(word.c_str()) );

			//std::unique_ptr<BaseEnemy> enemy;

			//該当する敵の属性ごとに生成

				//敵を発生させる
			//x座標
			getline(line_stream, word, ',');
			enemysPos_.x = ( float ) std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			enemysPos_.y = ( float ) std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			enemysPos_.z = ( float ) std::atof(word.c_str());

			std::unique_ptr<EnemyNormal> enemy = std::make_unique<EnemyNormal>();
  			enemy->Initialize(model_,camera_);
			enemy->SetPlayer(player);

			enemy->translation = enemysPos_;
			enemys.push_back(std::move(enemy));

		}
		else if ( word.find("WAIT") == 0 )
		{
			getline(line_stream,word,',');

			//csvから何F待つのか
			waitTime = atoi(word.c_str());

			//待機開始
			waitFlag = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}
