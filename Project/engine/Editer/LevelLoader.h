#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <string>
#include <vector>
MYENGINE_SUPPRESS_WARNINGS_END

#include "DirectX12Math.h"

/**
 * @class LevelLoader.h
 * @brief blenderからjsonファイルを読み取るクラス
 */

// レベルデータ
struct LevelData {
	//オブジェクトデータ
	struct ObjectData {
		// ファイル名
		std::string fileName;
		// 平行移動
		MyMath::Vector3 translation;
		// 回転角
		MyMath::Vector3 rotation;
		// スケーリング
		MyMath::Vector3 scaling;
		int8_t pad1[ 4 ];
	};

	//カーブデータ
	struct CurveData {
		// 真ん中
		MyMath::Vector3 pointCeter;
		// 左ハンドル
		MyMath::Vector3 pointLeft;
		// 右ハンドル
		MyMath::Vector3 pointRight;

	};

	//アニメーションキーフレームデータ
	struct AnimData {
		//現在のフレーム
		float nowFrame;
		//座標
		MyMath::Vector3 trans;
		//回転
		MyMath::Vector3 rot;
	};

	struct EnemyData{
		// ファイル名
		std::string enemyType;
		// 平行移動
		MyMath::Vector3 translation;
		int8_t pad2[ 4 ];
	};

	// 3つ並べた制御点
	std::vector<MyMath::Vector3> points;

	//現在のトランスフォーム
	std::vector<MyMath::Vector3> nowTransform;
	//現在のフレーム
	std::vector<float>nowFrame;

	// オブジェクト配列
	std::vector<ObjectData> objects;
	std::vector<CurveData> curves;
	std::vector<AnimData> anims;
	std::vector<EnemyData> enemys;
};

// レベルデータのローダー
class LevelLoader {

private:
	// デフォルトの読み込みディレクトリ
	static const std::string defaultBaseDirectory;
	// ファイル拡張子
	static const std::string extension;

	static int16_t objectType;

public:// メンバ関数

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	[[nodiscard]]
	static LevelData* LoadFile(const std::string& fileName);

private:
	
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
	/// スプライン曲線(blenderベジェ用)
	/// </summary>
	/// <param name="points">制御点</param>
	/// <param name="t">t時間(0.0~1.0)</param>
	/// <returns>4点分のベジェ曲線の結果</returns>
	MyMath::Vector3 SplinePosition(std::vector<LevelData::CurveData>& points, float t,size_t startIndex = 1);

	/// <summary>
	/// スプライン曲線()
	/// </summary>
	/// <param name="points">アニメーションフレーム</param>
	/// <param name="t">全体の時間</param>
	/// <param name="startIndex">開始するフレーム(デフォルトは1)</param>
	/// <returns></returns>
	MyMath::Vector3 SplinePositionAnim(std::vector<LevelData::AnimData>& points, float t ,size_t startIndex = 1);

	/// <summary>
	/// 方向ベクトルを取得
	/// </summary>
	/// <param name="prevPoint">開始点</param>
	/// <param name="nextPoint">次の制御点</param>
	/// <returns>方向ベクトル</returns>
	MyMath::Vector3 CalcTangentPosition(const MyMath::Vector3& prevPoint, const MyMath::Vector3& nextPoint);
}
