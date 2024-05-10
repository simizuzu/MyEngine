#pragma once
#include "LevelLoader.h"

class KeyframeCamera
{
public:
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
	LevelData* keyframeData;
	std::vector<LevelData::AnimData> points;

private:
	//そのオブジェクトのゲーム内開始時間
	const float startTime = 1.0f;
	//終了時間
	float endTime = 0.0f;
	//1フレーム
	float frameTime = 1.0f;
	//現在の時間(フレーム数)
	float currentTime = 0.0f;

	bool isPlay = false;

	//座標
	std::vector<MyMath::Vector3> translation_;
	//回転
	std::vector<MyMath::Vector3> rotation_;
};

