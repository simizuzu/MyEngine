#include "LevelLoader.h"

#include <json.hpp>
#include <fstream>
#include <cassert>

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

			// 左ハンドル
			curveData.pointLeft.x = (float)transform["HandlePointL"][0];
			curveData.pointLeft.y = (float)transform["HandlePointL"][2];
			curveData.pointLeft.z = (float)transform["HandlePointL"][1];

			// 制御点
			curveData.pointCeter.x = (float)transform["ControlPoint"][0];
			curveData.pointCeter.y = (float)transform["ControlPoint"][2];
			curveData.pointCeter.z = (float)transform["ControlPoint"][1];

			// 右ハンドル
			curveData.pointRight.x = (float)transform["HandlePointR"][0];
			curveData.pointRight.y = (float)transform["HandlePointR"][2];
			curveData.pointRight.z = (float)transform["HandlePointR"][1];


			levelData->points.push_back(curveData.pointLeft);
			levelData->points.push_back(curveData.pointCeter);
			levelData->points.push_back(curveData.pointRight);
		}
	}

	for (nlohmann::json& keyframes : deserialized["animdata"]) {

		//要素追加
		levelData->anims.emplace_back(LevelData::AnimData{});
		//今追加した要素の参照を得る
		LevelData::AnimData& animData = levelData->anims.back();

		//トランスフォームのパラメータ読み込み
		nlohmann::json& transform = keyframes["keyframe"];

		//座標
		animData.trans.x = (float)transform["translation"][0];
		animData.trans.y = (float)transform["translation"][2];
		animData.trans.z = (float)transform["translation"][1];
		//回転角
		animData.rot.x = (float)transform["rotation"][0];
		animData.rot.y = (float)transform["rotation"][2];
		animData.rot.z = (float)transform["rotation"][1];

		levelData->nowFrame.push_back(animData.trans);
		levelData->nowFrame.push_back(animData.rot);
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

	MyMath::Vector3 BezierCurve(std::vector<LevelData::CurveData>& points, float t)
	{
		//制御点のindexをずらしていく処理
		float length = static_cast<float>(points.size());
		float progress = (length - 1) * t;
		float index = std::floor(progress);
		float weight = progress - index;


		/*if (Approximately(weight, 0.0f) && index >= length - 1)
		{
			index = length - 2;
			weight = 1;
		}*/

		//始点
		MyMath::Vector3 p0 = points[static_cast<size_t>(index)].pointCeter;
		//制御点1
		MyMath::Vector3 p1 = points[static_cast<size_t>(index)].pointRight;
		//制御点2
		MyMath::Vector3 p2 = points[static_cast<size_t>(index + 1.0f)].pointLeft;
		//終点
		MyMath::Vector3 p3 = points[static_cast<size_t>(index + 1.0f)].pointCeter;

		MyMath::Vector3 tangent0 = MyMathUtility::CalcTangentPosition(p0, p1);
		MyMath::Vector3 tangent1 = MyMathUtility::CalcTangentPosition(p1, p2);
		MyMath::Vector3 tangent2 = MyMathUtility::CalcTangentPosition(p2, p3);

		//ベジェ曲線を代入
		MyMath::Vector3 position = BezierGetPoint(p0, p1, p2, p3, weight);

		//補間された接戦ベクトル
		MyMath::Vector3 tangent = MyMathUtility::Lerp(tangent1, tangent2,weight);

		//ポジションに接戦ベクトルを適用して一を修正
		float scaleFactor = 1.0f;
		position += tangent * scaleFactor;

		return position;
	}

	MyMath::Vector3 HermiteGetPoint2(const MyMath::Vector3& p0, const MyMath::Vector3& p1, const MyMath::Vector3& v0, const MyMath::Vector3& v1, float t)
	{
		MyMath::Vector3 c0 = 2.0f * p0 + -2.0f * p1 + v0 + v1;
		MyMath::Vector3 c1 = -3.0f * p0 + 3.0f * p1 + -2.0f * v0 - v1;
		MyMath::Vector3 c2 = v0;
		MyMath::Vector3 c3 = p0;

		float t2 = t * t;
		float t3 = t2 * t;
		return c0 * t3 + c1 * t2 + c2 * t + c3;
	}

	MyMath::Vector3 CatmullRomSpline(std::vector<LevelData::CurveData>& points, float t)
	{

		float length = static_cast<float>(points.size());
		float progress = (length - 1) * t;
		float index = std::floor(progress);
		float weight = progress - index;

		if (Approximately(weight, 0.0f) && index >= length - 1)
		{
			index = length - 2;
			weight = 1;
		}

		MyMath::Vector3 p0 = points[static_cast<size_t>(index)].pointCeter;
		MyMath::Vector3 p1 = points[static_cast<size_t>(index + 1.0f)].pointCeter;
		MyMath::Vector3 p2;
		MyMath::Vector3 p3;

		if (index > 0.0f)
		{
			p2 = 0.5f * (points[static_cast<size_t>(index + 1.0f)].pointCeter - points[static_cast<size_t>(index - 1.0f)].pointCeter);
		}
		else
		{
			p2 = points[static_cast<size_t>(index + 1.0f)].pointCeter - points[static_cast<size_t>(index)].pointCeter;
		}

		if (index < length - 2.0f)
		{
			p3 = 0.5f * (points[static_cast<size_t>(index + 2.0f)].pointCeter - points[static_cast<size_t>(index)].pointCeter);
		}
		else
		{
			p3 = points[static_cast<size_t>(index + 1.0f)].pointCeter - points[static_cast<size_t>(index)].pointCeter;
		}

		MyMath::Vector3 tangent0 = MyMathUtility::CalcTangentPosition(p0, p1);
		MyMath::Vector3 tangent1 = MyMathUtility::CalcTangentPosition(p1, p2);
		MyMath::Vector3 tangent2 = MyMathUtility::CalcTangentPosition(p2, p3);

		MyMath::Vector3 position = HermiteGetPoint2(p0, p1, p2, p3, weight);
		MyMath::Vector3 tangent = MyMathUtility::Lerp(tangent1, tangent2, weight);

		//ポジションに接戦ベクトルを適用して一を修正
		float scaleFactor = 1.0f;
		position += tangent * scaleFactor;


		return position;
	}

	MyMath::Vector3 CalcTangentPosition(const MyMath::Vector3& prevPoint, const MyMath::Vector3& nextPoint)
	{
		//方向ベクトルを取得
		MyMath::Vector3 calcVec = nextPoint - prevPoint;
		return calcVec.normalize();
	}

	//MyMath::Vector3 AnimationKeyframe(std::vector<LevelData::AnimData>& nowFrame, float wholeFrame)
	//{
	//	
	//}
}
