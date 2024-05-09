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
		float frame;
		//座標
		MyMath::Vector3 translation;
		//回転
		MyMath::Vector3 rotation;
		//スケール
		MyMath::Vector3 scaling;
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
	std::vector<MyMath::Vector3> transforms;
	//現在のフレーム
	std::vector<float>keyframes;

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
};

namespace MyMathUtility {
	/// <summary>
	/// スプライン曲線(blenderベジェ用)
	/// </summary>
	/// <param name="points">制御点</param>
	/// <param name="t">t時間(0.0~1.0)</param>
	/// <returns>4点分のベジェ曲線の結果</returns>
	MyMath::Vector3 SplinePosition(std::vector<LevelData::CurveData>& points, float t,size_t startIndex = 1);

	/// <summary>
	/// スプライン曲線(blenderアニメーション用)
	/// </summary>
	/// <param name="points">制御点</param>
	/// <param name="t"></param>
	/// <param name="startIndex"></param>
	/// <returns></returns>
	/*MyMath::Vector3 SplinePosition(std::vector<LevelData::AnimData>& points, float frame);

	MyMath::Vector3 InterpolateControlPoints(const std::vector<LevelData::AnimData>& points,int frame,std::function<MyMath::Vector3(const LevelData::AnimData&,const LevelData::AnimData&,float)> interpolator);

	void CalculateTangents(LevelData::AnimData& p0,LevelData::AnimData& p1,LevelData::AnimData& p2,LevelData::AnimData& p3,MyMath::Vector3& m0,MyMath::Vector3& m1);*/
}
