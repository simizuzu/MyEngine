#pragma once

#include <string>
#include <vector>
#include "DirectX12Math.h"

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

	struct CurveData {
		// 真ん中
		MyMath::Vector3 pointCeter;
		// 左ハンドル
		MyMath::Vector3 pointLeft;
		// 右ハンドル
		MyMath::Vector3 pointRight;
		// 3つ並べた制御点
		std::vector<MyMath::Vector3> points;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
	std::vector<CurveData> curves;
};

// レベルデータのローダー
class LevelLoader {

private:
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

	static int16_t objectType;

public:// メンバ関数

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	[[nodiscard]]
	static LevelData* LoadFile(const std::string& fileName);
};
