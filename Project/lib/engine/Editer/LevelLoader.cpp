#include "LevelLoader.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <json.hpp>
#include <fstream>
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @file LevelLoader.cpp
 * @brief LevelLoaderの処理について書いてあります
 */

const std::string LevelLoader::kDefaultBaseDirectory = "Resources/levels/";
const std::string LevelLoader::kExtension = ".json";

[[nodiscard]]
LevelData* LevelLoader::LoadFile(const std::string& fileName)
{
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// typeがMESHの時
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][0];
			objectData.translation.z = -objectData.translation.z;
			// 回転角
			objectData.rotation.x = (float)transform["rotation"][1];
			objectData.rotation.x = -objectData.rotation.x;
			objectData.rotation.y = (float)transform["rotation"][2];
			objectData.rotation.y = -objectData.rotation.y;
			objectData.rotation.z = (float)transform["rotation"][0];
			// スケーリング
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			// TODO: コライダーのパラメータ読み込み
		}

		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
	}

	// "curves"の制御点の X,Y,Z を走査
	for (nlohmann::json& curves : deserialized["curves"]) {

		for (nlohmann::json& curve : curves["curve"]) {
			// 要素追加
			levelData->curves.emplace_back(LevelData::CurveData{});
			// 今追加した要素の参照を得る
			LevelData::CurveData& curveData = levelData->curves.back();

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = curve;

			// 制御点
			curveData.pointCeter.x = (float)transform["ControlPoint"][0];
			curveData.pointCeter.y = (float)transform["ControlPoint"][2];
			curveData.pointCeter.z = (float)transform["ControlPoint"][1];

			levelData->points.push_back(curveData.pointCeter);
		}
	}

	for (nlohmann::json& keyframes : deserialized["animdata"]) {

		//要素追加
		levelData->anims.emplace_back(LevelData::AnimData{});
		//今追加した要素の参照を得る
		LevelData::AnimData& animData = levelData->anims.back();

		//トランスフォームのパラメータ読み込み
		nlohmann::json& transform = keyframes["keyframe"];

		//現在のフレーム
		animData.nowFrame = (float)transform["nowframe"];
		//座標
		animData.trans.x = (float)transform["translation"][0];
		animData.trans.y = (float)transform["translation"][1];
		animData.trans.z = (float)transform["translation"][2];
		//回転角
		animData.rot.x = (float)transform["rotation"][0];
		animData.rot.y = (float)transform["rotation"][1];
		animData.rot.z = (float)transform["rotation"][2];

		levelData->nowFrame.push_back(animData.nowFrame);
		levelData->nowTransform.push_back(animData.trans);
		levelData->nowTransform.push_back(animData.rot);
	}

	return levelData;
}

namespace MyMathUtility {
	MyMath::Vector3 BezierGetPoint(MyMath::Vector3 p0, MyMath::Vector3 p1, MyMath::Vector3 p2, MyMath::Vector3 p3, float t)
	{
		MyMath::Vector3 c0 = Lerp(p0, p1, t);
		MyMath::Vector3 c1 = Lerp(p1, p2, t);
		MyMath::Vector3 c2 = Lerp(p2, p3, t);
		MyMath::Vector3 c3 = Lerp(c0, c1, t);
		MyMath::Vector3 c4 = Lerp(c1, c2, t);

		return Lerp(c3, c4, t);
	}

	MyMath::Vector3 SplinePosition(std::vector<LevelData::CurveData>& points, float t, size_t startIndex)
	{
		size_t n = points.size() - 2;

		if (startIndex > n) return points[n].pointCeter; //Pnの値を返す
		if (startIndex < 1) return points[1].pointCeter; //Pnの値を返す

		//始点
		MyMath::Vector3 p0 = points[startIndex - 1].pointCeter;
		//制御点1
		MyMath::Vector3 p1 = points[startIndex].pointCeter;
		//制御点2
		MyMath::Vector3 p2 = points[startIndex + 1].pointCeter;
		//終点
		MyMath::Vector3 p3 = points[startIndex + 2].pointCeter;

		//ベジェ曲線を代入
		MyMath::Vector3 position = MyMathUtility::HermiteGetPoint(p0, p1, p2, p3, t);

		return position;
	}

	MyMath::Vector3 SplinePositionAnim(std::vector<LevelData::AnimData>& points,float t,size_t startIndex)
	{
		size_t n = points.size() - 2;

		if ( startIndex > n ) return points[ n ].trans; //Pnの値を返す
		if ( startIndex < 1 ) return points[ 1 ].trans; //Pnの値を返す

		//始点
		MyMath::Vector3 p0 = points[ startIndex - 1 ].trans;
		//制御点1
		MyMath::Vector3 p1 = points[ startIndex ].trans;
		//制御点2
		MyMath::Vector3 p2 = points[ startIndex + 1 ].trans;
		//終点
		MyMath::Vector3 p3 = points[ startIndex + 2 ].trans;

		//ベジェ曲線を代入
		MyMath::Vector3 position = MyMathUtility::HermiteGetPoint(p0,p1,p2,p3,t);

		return position;
	}

	MyMath::Vector3 CalcTangentPosition(const MyMath::Vector3& prevPoint, const MyMath::Vector3& nextPoint)
	{
		//方向ベクトルを取得
		MyMath::Vector3 calcVec = nextPoint - prevPoint;
		return calcVec.normalize();
	}

	/*MyMath::Vector3 AnimationKeyframe(std::vector<LevelData::AnimData>& nowFrame, float wholeFrame)
	{

	}*/
}
