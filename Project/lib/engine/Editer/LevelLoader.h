﻿#pragma once

#include <string>
#include <vector>
#include "Vector3.h"

// レベルデータ
struct LevelData {

	struct ObjectData {
		// ファイル名
		std::string fileName;
		// 平行移動
		MyMath::Vector3 translation;
		// 回転角
		MyMath::Vector3 rotation;
		// スケーリング
		MyMath::Vector3 scaling;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
};

// レベルデータのローダー
class LevelLoader {

public:// 定数
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

public:// メンバ関数

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	static LevelData* LoadFile(const std::string& fileName);
};
