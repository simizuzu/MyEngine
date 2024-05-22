#pragma once
#include "LevelLoader.h"
#include "Camera.h"

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
	void Update(Camera* camera);

	/// <summary>
	/// キーフレームを再生する
	/// </summary>
	void IsPlayAnimation();

private:
	Keyframe* keyframeData;
	
private:
	float animationTime = 1.0f / 60.0f;
	MyMath::Vector3 translate;

private:
//代入演算子削除
	KeyframeObject& operator=(const KeyframeObject&) = delete;
	//コピーコンストラクタ削除
	KeyframeObject(const KeyframeObject&) = delete;
};

