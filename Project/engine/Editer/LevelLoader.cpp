#include "LevelLoader.h"
#include "WorldTransform.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <json.hpp>
#include <fstream>
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @file LevelLoader.cpp
 * @brief LevelLoaderの処理について書いてあります
 */

const std::string LevelLoader::defaultBaseDirectory = "Resources/levels/";
const std::string LevelLoader::extension = ".json";

std::string LevelLoader::cameraName;
std::string LevelLoader::meshName;

[[nodiscard]]
LevelData* LevelLoader::LoadFile(const std::string& fileName)
{
	// 連結してフルパスを得る
	const std::string fullpath = defaultBaseDirectory + fileName + extension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if ( file.fail() )
	{
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized[ "name" ].is_string());

	// "name"を文字列として取得
	std::string name = deserialized[ "name" ].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for ( nlohmann::json& object : deserialized[ "objects" ] )
	{
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object[ "type" ].get<std::string>();

		// typeがMESHの時
		if ( type.compare("MESH") == 0 )
		{
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if ( object.contains("file_name") )
			{
				// ファイル名
				objectData.fileName = object[ "file_name" ];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object[ "transform" ];
			// 平行移動
			objectData.translation.x = ( float ) transform[ "translation" ][ 0 ];
			objectData.translation.y = ( float ) transform[ "translation" ][ 2 ];
			objectData.translation.z = ( float ) transform[ "translation" ][ 1 ];
			// 回転角
			objectData.rotation.x = ( float ) transform[ "rotation" ][ 1 ];
			objectData.rotation.x = -objectData.rotation.x;
			objectData.rotation.y = ( float ) transform[ "rotation" ][ 2 ];
			objectData.rotation.y = -objectData.rotation.y;
			objectData.rotation.z = ( float ) transform[ "rotation" ][ 0 ];
			// スケーリング
			objectData.scaling.x = ( float ) transform[ "scaling" ][ 1 ];
			objectData.scaling.y = ( float ) transform[ "scaling" ][ 2 ];
			objectData.scaling.z = ( float ) transform[ "scaling" ][ 0 ];

			// TODO: コライダーのパラメータ読み込み
		}

		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if ( object.contains("children") )
		{

		}
	}

	// "objects"からfilenameで指定した敵の X,Y,Z を走査
	for ( nlohmann::json& enemys : deserialized[ "objects" ] )
	{
		// 要素追加
		levelData->enemys.emplace_back(LevelData::EnemyData{});
		// 今追加した要素の参照を得る
		LevelData::EnemyData& enemyData = levelData->enemys.back();

		//敵の名前を検索
		if ( enemys.contains("file_name") )
		{
			// ファイル名
			enemyData.enemyType = enemys[ "file_name" ];
		}

		nlohmann::json& transform = enemys[ "transform" ];
		// 平行移動
		enemyData.translation.x = ( float ) transform[ "translation" ][ 0 ];
		enemyData.translation.y = ( float ) transform[ "translation" ][ 2 ];
		enemyData.translation.z = ( float ) transform[ "translation" ][ 1 ];
	}

	// "curves"の制御点の X,Y,Z を走査
	for ( nlohmann::json& curves : deserialized[ "curves" ] )
	{
		for ( nlohmann::json& curve : curves[ "curve" ] )
		{
			// 要素追加
			levelData->curves.emplace_back(LevelData::CurveData{});
			// 今追加した要素の参照を得る
			LevelData::CurveData& curveData = levelData->curves.back();

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = curve;

			// 制御点
			curveData.pointCeter.x = ( float ) transform[ "ControlPoint" ][ 0 ];
			curveData.pointCeter.y = ( float ) transform[ "ControlPoint" ][ 2 ];
			curveData.pointCeter.z = ( float ) transform[ "ControlPoint" ][ 1 ];

			levelData->points.push_back(curveData.pointCeter);
		}
	}

	return levelData;
}

Keyframe* LevelLoader::LoadKeyframe(const std::string& fileName,float ticsPerSecond)
{
	// 連結してフルパスを得る
	const std::string fullpath = defaultBaseDirectory + fileName + extension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if ( file.fail() )
	{
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized[ "name" ].is_string());

	// "name"を文字列として取得
	std::string name = deserialized[ "name" ].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	Keyframe* keyframe = new Keyframe();

	//"animations""MESH"から frame,X,Y,Z要素を走査(カメラ用)
	for ( nlohmann::json& animations : deserialized[ "animations" ] )
	{
		assert(animations.contains("type"));
		//種別を取得
		std::string type = animations[ "type" ].get<std::string>();

		//typeがCAMERAの時
		if ( type.compare("CAMERA") == 0 )
		{
			cameraName = animations[ "name" ].get<std::string>();

			Keyframe::CameraKeyframe& keyframeName = keyframe->cameraKeyframe[ cameraName ];

			float lastFrame = 0.0f;
			//最後のフレームを取得
			if ( animations.contains("last_frame") )
			{
				lastFrame = animations[ "last_frame" ];
			}

			keyframeName.duration = lastFrame;

			//何フレームで進むかを決める
			keyframeName.ticsPerSecond = ticsPerSecond;

			for ( nlohmann::json& animedata : animations[ "animedata" ] )
			{
				float frame = 0.0f;

				//jsonファイルからフレーム数を検索
				if ( animedata.contains("frame") )
				{
					//フレーム数を取得
					frame = animedata[ "frame" ];
				}

				Keyframe::KeyframeVector3 translation;
				Keyframe::KeyframeQuaternion rotation;

				//平行移動 (blender上ではZ方向とY方向が逆のため入れ替える)
				translation.value.x = ( float ) animedata[ "translation" ][ 0 ];
				translation.value.y = ( float ) animedata[ "translation" ][ 2 ];
				translation.value.z = ( float ) animedata[ "translation" ][ 1 ];
				translation.time = frame;
				keyframeName.translate.push_back(translation);

				//回転
				rotation.value.w = ( float ) animedata[ "rotation" ][ 0 ];
				rotation.value.x = ( float ) animedata[ "rotation" ][ 2 ];
				rotation.value.x = -rotation.value.x;
				rotation.value.y = ( float ) animedata[ "rotation" ][ 3 ];
				rotation.value.z = ( float ) animedata[ "rotation" ][ 1 ];
				rotation.time = frame;
				keyframeName.rotate.push_back(rotation);
			}
		}
	}

	//"animations""MESH"から frame,X,Y,Z要素を走査(カメラ用)
	for ( nlohmann::json& animations : deserialized[ "animations" ] )
	{
		assert(animations.contains("type"));
		//種別を取得
		std::string type = animations[ "type" ].get<std::string>();

		//typeがMESHの時
		if ( type.compare("MESH") == 0 )
		{
			meshName = animations[ "name" ].get<std::string>();

			Keyframe::MeshKeyframe& keyframeName = keyframe->meshKeyframe[ meshName ];

			float lastFrame = 0.0f;
			//最後のフレームを取得
			if ( animations.contains("last_frame") )
			{
				lastFrame = animations["last_frame" ];
			}

			//全体フレームに最後のフレームを代入する
			keyframeName.duration = lastFrame;

			//何フレームで進むかを決める
			keyframeName.ticsPerSecond = ticsPerSecond;

			for ( nlohmann::json& animedata : animations[ "animedata" ] )
			{
				float frame = 0.0f;

				//jsonファイルからフレーム数を検索
				if ( animedata.contains("frame") )
				{
					//フレーム数を取得
					frame = animedata[ "frame" ];
				}

				Keyframe::KeyframeVector3 translation,scaling;
				Keyframe::KeyframeQuaternion rotation;

				//平行移動 (blender上ではZ方向とY方向が逆のため入れ替える)
				translation.value.x = ( float ) animedata[ "translation" ][ 0 ];
				translation.value.y = ( float ) animedata[ "translation" ][ 2 ];
				translation.value.z = ( float ) animedata[ "translation" ][ 1 ];
				translation.time = frame;
				keyframeName.translate.push_back(translation);

				//回転 (XとYで座標系を変更)
				rotation.value.w = ( float ) animedata[ "rotation" ][ 0 ];
				rotation.value.x = ( float ) animedata[ "rotation" ][ 2 ];
				rotation.value.x = -rotation.value.x;
				rotation.value.y = ( float ) animedata[ "rotation" ][ 3 ];
				rotation.value.z = ( float ) animedata[ "rotation" ][ 1 ];
				rotation.time = frame;
				keyframeName.rotate.push_back(rotation);

				//スケール
				scaling.value.x = ( float ) animedata[ "scaling" ][ 0 ];
				scaling.value.y = ( float ) animedata[ "scaling" ][ 1 ];
				scaling.value.z = ( float ) animedata[ "scaling" ][ 2 ];
				scaling.time = frame;
				keyframeName.scale.push_back(scaling);
			}
		}
	}

	return keyframe;
}

namespace MyMathUtility
{
	MyMath::Vector3 SplinePosition(std::vector<LevelData::CurveData>& points,float t,size_t startIndex)
	{
		size_t n = points.size() - 2;

		if ( startIndex > n ) return points[ n ].pointCeter; //Pnの値を返す
		if ( startIndex < 1 ) return points[ 1 ].pointCeter; //Pnの値を返す

		//始点
		MyMath::Vector3 p0 = points[ startIndex - 1 ].pointCeter;
		//制御点1
		MyMath::Vector3 p1 = points[ startIndex ].pointCeter;
		//制御点2
		MyMath::Vector3 p2 = points[ startIndex + 1 ].pointCeter;
		//終点
		MyMath::Vector3 p3 = points[ startIndex + 2 ].pointCeter;

		//ベジェ曲線を代入
		MyMath::Vector3 position = MyMathUtility::HermiteGetPoint(p0,p1,p2,p3,t);

		return position;
	}

	MyMath::Vector3 CalculateValueLerp(const std::vector<Keyframe::KeyframeVector3>& keyframes,float time)
	{
		//必ずキーがないものはassertをかける
		assert(!keyframes.empty());
		//キーが1つ場合または時刻がキーフレーム前なら最初の値とする
		if ( keyframes.size() == 1 || time <= keyframes[ 0 ].time )
		{
			return keyframes[ 0 ].value;
		}

		for ( size_t index = 0; index < keyframes.size() - 1; ++index )
		{
			size_t nextIndex = index + 1;

			//indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
			if ( keyframes[ index ].time <= time && time <= keyframes[ nextIndex ].time )
			{
				//範囲内を補間する
				float t = ( time - keyframes[ index ].time ) / ( keyframes[ nextIndex ].time - keyframes[ index ].time );
				return MyMathUtility::Lerp(keyframes[ index ].value,keyframes[ nextIndex ].value,t);
			}
		}

		//一番後の時刻よりも後の値になったとき最後の値を返す
		return (*keyframes.rbegin()).value;
	}
	MyMath::Quaternion CalculateValueSlerp(const std::vector<Keyframe::KeyframeQuaternion>& keyframes,float time)
	{
		//必ずキーがないものはassertをかける
		assert(!keyframes.empty());
		//キーが1つ場合または時刻がキーフレーム前なら最初の値とする
		if ( keyframes.size() == 1 || time <= keyframes[ 0 ].time )
		{
			return keyframes[ 0 ].value;
		}

		for ( size_t index = 0; index < keyframes.size() - 1; ++index )
		{
			size_t nextIndex = index + 1;

			//indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
			if ( keyframes[ index ].time <= time && time <= keyframes[ nextIndex ].time )
			{
				//範囲内を補間する
				float t = ( time - keyframes[ index ].time ) / ( keyframes[ nextIndex ].time - keyframes[ index ].time );
				return MyMathUtility::Slerp(keyframes[ index ].value,keyframes[ nextIndex ].value,t);
			}
		}

		//一番後の時刻よりも後の値になったとき最後の値を返す
		return ( *keyframes.rbegin() ).value;
	}
}
