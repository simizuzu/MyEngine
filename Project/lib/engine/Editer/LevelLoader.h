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

	};

	// 3つ並べた制御点
	std::vector<MyMath::Vector3> points;

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

namespace MyMathUtility {
	/// <summary>
	/// 4点分のベジェ曲線
	/// </summary>
	/// <param name="p0">始点</param>
	/// <param name="p1">制御点1</param>
	/// <param name="v0">制御点2</param>
	/// <param name="v1">終点</param>
	/// <param name="t">時間</param>
	/// <returns>ベジェ曲線の計算結果</returns>
	MyMath::Vector3 BezierGetPoint(MyMath::Vector3 p0, MyMath::Vector3 p1, MyMath::Vector3 p2, MyMath::Vector3 p3, float t);

	/// <summary>
	/// ベジェ曲線
	/// </summary>
	/// <param name="points">制御点</param>
	/// <param name="t">t時間(0.0~1.0)</param>
	/// <returns>4点分のベジェ曲線の結果</returns>
	MyMath::Vector3 BezierCurve(std::vector<LevelData::CurveData>& points, float t);

	/// <summary>
/// エルミート曲線
/// </summary>
/// <param name="p0">制御点</param>
/// <param name="p1">制御点</param>
/// <param name="v0">制御点</param>
/// <param name="v1">制御点</param>
/// <param name="t">時間(0.0～1.0)</param>
/// <returns></returns>
	MyMath::Vector3 HermiteGetPoint2(const MyMath::Vector3& p0, const MyMath::Vector3& p1, const MyMath::Vector3& v0, const MyMath::Vector3& v1, float t);

	/// <summary>
	/// キャットムルーロムスプライン 
	/// </summary>
	/// <param name="points">制御点</param>
	/// <param name="t">時間(0.0～1.0)</param>
	MyMath::Vector3 CatmullRomSpline(std::vector<LevelData::CurveData>& points, float t);
}
