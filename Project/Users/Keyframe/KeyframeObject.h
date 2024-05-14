#pragma once
#include "LevelLoader.h"

class KeyframeObject
{
public:
	KeyframeObject() = default;
	~KeyframeObject() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(LevelData* animeData);

	/// <summary>
	/// キーフレームを再生する
	/// </summary>
	void IsPlayAnimation();

private:
	Keyframe* keyframeData;
	std::vector<LevelData::AnimData> points;

	Keyframe::KeyframeVector3 node;
	//Keyframe::NodeAnimation nodeAnimation;
	
private:
	//そのオブジェクトのゲーム内開始時間
	const float startTime = 0.0f;
	//終了時間
	float endTime = 0.0f;
	//1フレーム
	float frameTime = 1.0f;
	//現在の時間(フレーム数)
	float currentTime = 0.0f;

	bool isPlay = false;

	int8_t pad[ 7 ];

private:
//代入演算子削除
	KeyframeObject& operator=(const KeyframeObject&) = delete;
	//コピーコンストラクタ削除
	KeyframeObject(const KeyframeObject&) = delete;
};

