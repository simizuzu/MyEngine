#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <vector>
#include <string>
#include <map>
MYENGINE_SUPPRESS_WARNINGS_END

#include "DirectX12Math.h"

/**
 * @class LevelLoader.h
 * @brief blenderからjsonファイルを読み取るクラス
 */

//キーフレームデータ
struct Keyframe
{
	//１つのキーフレーム情報が入っている
	struct KeyframeVector3
	{
		MyMath::Vector3 value;	//キーフレームの値
		float time;				//キーフレームの時刻
	};

	struct KeyframeQuaternion
	{
		MyMath::Quaternion value;	//キーフレームの値
		float time;				//キーフレームの時刻
	};

	//キーフレーム内の各トランスフォーム
	struct MeshKeyframe
	{
		std::vector<KeyframeVector3> translate;	//座標
		std::vector<KeyframeQuaternion> rotate;	//回転
		std::vector<KeyframeVector3> scale;		//スケール
		float duration;	//アニメーション全体の尺（単位は秒）
		float ticsPerSecond; //何フレームで進むか(30F,60F,120F,...)
	};

	struct CameraKeyframe
	{
		std::vector<KeyframeVector3> translate;	//座標
		std::vector<KeyframeQuaternion> rotate;	//回転
		float duration;	//アニメーション全体の尺（単位は秒）
		float ticsPerSecond; //何フレームで進むか(30F,60F,120F,...)
	};

	std::map<std::string,MeshKeyframe> meshKeyframe;
	std::map<std::string,CameraKeyframe> cameraKeyframe;
};

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
		MyMath::Quaternion rotation;
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
	std::vector<MyMath::Vector3> transformObjects;
	std::vector<MyMath::Vector3> transformCamera;
	//現在のフレーム
	std::vector<float>keyframeObjects;
	std::vector<float>keyframeCamera;

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

	static std::string cameraName;
	static std::string meshName;

public:// メンバ関数

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	[[nodiscard]]
	static LevelData* LoadFile(const std::string& fileName);

	[[nodiscard]]
	static Keyframe* LoadKeyframe(const std::string& fileName, float ticsPerSecond);
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
	/// 任意の時刻の値を取得する関数(線形補間)
	/// </summary>
	/// <param name="keyframes">キーフレーム</param>
	/// <param name="time">時間</param>
	/// <returns>任意の時刻の値</returns>
	MyMath::Vector3 CalculateValueLerp(const std::vector<Keyframe::KeyframeVector3>& keyframes, float time);

	/// <summary>
	/// 任意の時刻の値を取得する関数(球面線形補間)
	/// </summary>
	/// <param name="keyframes">キーフレーム</param>
	/// <param name="time">時間</param>
	/// <returns>時刻の</returns>
	MyMath::Quaternion CalculateValueSlerp(const std::vector<Keyframe::KeyframeQuaternion>& keyframes,float time);
}
