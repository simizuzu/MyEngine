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
			curveData.pointLeft.x = (float)transform["HandlePointL"][1];
			curveData.pointLeft.y = (float)transform["HandlePointL"][2];
			curveData.pointLeft.z = (float)transform["HandlePointL"][0];
			// 制御点
			curveData.pointCeter.x = (float)transform["ControlPoint"][1];
			curveData.pointCeter.y = (float)transform["ControlPoint"][2];
			curveData.pointCeter.z = (float)transform["ControlPoint"][0];
			// 右ハンドル
			curveData.pointRight.x = (float)transform["HandlePointR"][1];
			curveData.pointRight.y = (float)transform["HandlePointR"][2];
			curveData.pointRight.z = (float)transform["HandlePointR"][0];

			levelData->points.push_back(curveData.pointLeft);
			levelData->points.push_back(curveData.pointCeter);
			levelData->points.push_back(curveData.pointRight);
		}
	}
	return levelData;
}
