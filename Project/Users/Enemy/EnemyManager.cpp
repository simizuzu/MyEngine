#include "EnemyManager.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <fstream>
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

void EnemyManager::Initialize()
{
}

void EnemyManager::Update()
{
}

void EnemyManager::Draw()
{
}

void EnemyManager::EnemyNormalEmit()
{
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

void EnemyManager::UpdateEnemyPopCommands()
{
	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while ( getline(enemyPospCommands,line) )
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

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//[,]区切りで行の先頭文字列を取得
		getline(line_stream,word,',');

		//["//"]から始まる行はコメント
		if ( word.find("//") == 0 )
		{
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if ( word.find("POP") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			enemysPos_.x = (float )std::atof(word.c_str());

			//x座標
			getline(line_stream,word,',');
			enemysPos_.y = ( float ) std::atof(word.c_str());

			//x座標
			getline(line_stream,word,',');
			enemysPos_.z = ( float ) std::atof(word.c_str());

			//敵を発生させる
			//敵発生(enemysPos_);
		}
		else if ( word.find("POP") == 0 )
		{
			getline(line_stream,word,',');

			//csvから何F待つのか
			waitTime = atoi(word.c_str());

			//待機開始
			waitFlag =true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}
