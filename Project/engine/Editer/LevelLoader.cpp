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

	//"animations""MESH"から frame,X,Y,Z要素を走査(カメラ用)
	for ( nlohmann::json& animations : deserialized[ "animations" ] )
	{
		assert(animations.contains("type"));
		//種別を取得
		std::string type = animations[ "type" ].get<std::string>();

		//typeがCAMERAの時
		if ( type.compare("CAMERA") == 0 )
		{
			for ( nlohmann::json& animedata : animations[ "animedata" ] )
			{
				//要素追加
				levelData->anims.emplace_back(LevelData::AnimData{});
				//今追加した要素の参照を得る
				LevelData::AnimData& animeData = levelData->anims.back();

				//jsonファイルからフレーム数を検索
				if ( animedata.contains("frame") )
				{
					//フレーム数を取得
					animeData.frame = animedata[ "frame" ];
				}

				//平行移動 (blender上ではZ方向とY方向が逆のため入れ替える)
				animeData.translation.x = ( float ) animedata[ "translation" ][ 0 ];
				animeData.translation.y = ( float ) animedata[ "translation" ][ 2 ];
				animeData.translation.z = ( float ) animedata[ "translation" ][ 1 ];

				//回転 (XとYで座標系を変更)
				animeData.rotation.x = ( float ) animedata[ "rotation" ][ 0 ];
				animeData.rotation.x = -animeData.rotation.x;
				animeData.rotation.y = ( float ) animedata[ "rotation" ][ 1 ];
				animeData.rotation.x = -animeData.rotation.y;
				animeData.rotation.z = ( float ) animedata[ "rotation" ][ 2 ];

				//各要素を登録する
				levelData->keyframeCamera.push_back(animeData.frame);
				levelData->transformCamera.push_back(animeData.translation);
				levelData->transformCamera.push_back(animeData.rotation);
			}
		}
	}

	//"animations""MESH"から frame,X,Y,Z要素を走査(オブジェクト用)
	for ( nlohmann::json& animations : deserialized[ "animations" ] )
	{
		assert(animations.contains("type"));
		//種別を取得
		std::string type = animations[ "type" ].get<std::string>();

		//typeがMESHの時
		if ( type.compare("MESH") == 0 )
		{
			for ( nlohmann::json& animedata : animations[ "animedata" ] )
			{
				//要素追加
				levelData->anims.emplace_back(LevelData::AnimData{});
				//今追加した要素の参照を得る
				LevelData::AnimData& animeData = levelData->anims.back();

				//jsonファイルからフレーム数を検索
				if ( animedata.contains("frame") )
				{
					//フレーム数を取得
					animeData.frame = animedata[ "frame" ];
				}

				//平行移動 (blender上ではZ方向とY方向が逆のため入れ替える)
				animeData.translation.x = ( float ) animedata[ "translation" ][ 0 ];
				animeData.translation.y = ( float ) animedata[ "translation" ][ 2 ];
				animeData.translation.z = ( float ) animedata[ "translation" ][ 1 ];

				//回転 (XとYで座標系を変更)
				animeData.rotation.x = ( float ) animedata[ "rotation" ][ 0 ];
				animeData.rotation.x = -animeData.rotation.x;
				animeData.rotation.y = ( float ) animedata[ "rotation" ][ 1 ];
				animeData.rotation.x = -animeData.rotation.y;
				animeData.rotation.z = ( float ) animedata[ "rotation" ][ 2 ];

				//スケーリング
				animeData.scaling.x = ( float ) animedata[ "scaling" ][ 0 ];
				animeData.scaling.y = ( float ) animedata[ "scaling" ][ 1 ];
				animeData.scaling.z = ( float ) animedata[ "scaling" ][ 2 ];

				//各要素を登録する
				levelData->keyframeObjects.push_back(animeData.frame);
				levelData->transformObjects.push_back(animeData.translation);
				levelData->transformObjects.push_back(animeData.rotation);
				levelData->transformObjects.push_back(animeData.scaling);
			}
		}
	}

	return levelData;
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

	//MyMath::Vector3 SplinePosition(std::vector<LevelData::AnimData>& points,float frame)
	//{
	//	MyMath::Vector3 defaultNum = {0,0,0};
	//	// フレーム数に基づいて制御点間の補間を行う
	//	for ( size_t i = 1; i < points.size(); ++i )
	//	{
	//		// フレーム数が範囲内の制御点を見つける
	//		if ( frame >= points[ i - 1 ].frame && frame <= points[ i ].frame )
	//		{
	//			// フレーム間の相対位置を計算
	//			float t = static_cast< float >( frame - points[ i - 1 ].frame ) /
	//				( points[ i ].frame - points[ i - 1 ].frame );
	//			MyMath::Vector3 m0,m1;
	//			if ( i == 1 )
	//			{ // 最初のセグメントの場合はm0を計算
	//				m0 = { 0.0f, 0.0f, 0.0f }; // 開始点では接線を0と仮定
	//			}
	//			else
	//			{
	//				CalculateTangents(points[ i - 2 ],points[ i - 1 ],points[ i ],points[ i + 1 ],m0,m1);
	//			}
	//			if ( i == points.size() - 1 )
	//			{ // 最後のセグメントの場合はm1を計算
	//				m1 = { 0.0f, 0.0f, 0.0f }; // 終了点では接線を0と仮定
	//			}
	//			// エルミート曲線に基づいて補間を行う
	//			return MyMathUtility::HermiteGetPoint(points[ i - 1 ].translation,points[ i ].translation, m0,m1,t);
	//		}
	//	}
	//	return defaultNum; // フレームが範囲外の場合はデフォルト値を返す
	//}

	//MyMath::Vector3 InterpolateControlPoints(const std::vector<LevelData::AnimData>& points,int frame,std::function<MyMath::Vector3(const LevelData::AnimData&,const LevelData::AnimData&,float)> interpolator)
	//{
	//	MyMath::Vector3 defaultNum = {0,0,0};
	//	for ( size_t i = 1; i < points.size(); ++i )
	//	{
	//		if ( frame >= points[ i - 1 ].frame && frame <= points[ i ].frame )
	//		{
	//			// 現在のフレームが制御点の範囲内にある場合
	//			// tの値を計算し、指定された補間関数を使用して補間を行う
	//			float t = static_cast< float >( frame - points[ i - 1 ].frame ) /
	//				( points[ i ].frame - points[ i - 1 ].frame );
	//			return interpolator(points[ i - 1 ],points[ i ],t);
	//		}
	//	}
	//	// フレームが制御点の範囲外の場合、デフォルトの値を返す
	//	return defaultNum;
	//}

	//void CalculateTangents(LevelData::AnimData& p0,LevelData::AnimData& p1,LevelData::AnimData& p2,LevelData::AnimData& p3,MyMath::Vector3& m0,MyMath::Vector3& m1)
	//{
	//	float dt1 = ( p2.frame - p1.frame ) / 60.0f;
	//	float dt0 = ( p1.frame - p0.frame ) / 60.0f;
	//	float dt2 = ( p3.frame - p2.frame ) / 60.0f;

	//	m0 = { ( p2.translation.x - p0.translation.x ) / ( dt0 + dt1 ),
	//		   ( p2.translation.y - p0.translation.y ) / ( dt0 + dt1 ),
	//		   ( p2.translation.z - p0.translation.z ) / ( dt0 + dt1 )};

	//	m1 = { ( p3.translation.x - p1.translation.x ) / ( dt1 + dt2 ),
	//	       ( p3.translation.y - p1.translation.y ) / ( dt1 + dt2 ),
	//	       ( p3.translation.z - p1.translation.z ) / ( dt1 + dt2 ) };

	//	m1 = ( p3.translation - p1.translation ) / ( dt1 + dt2);
	//}
}
