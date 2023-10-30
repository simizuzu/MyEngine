#pragma once
#include "Sprite.h"
#include "TextureManager.h"
#include "Input.h"

/**
 * @class TransitionScene.h
 * @brief BaseSceneにて管理するシーン遷移クラス
 */

/// <summary>
/// シーン遷移用クラス
/// </summary>
class TransitionScene
{
private:
	const MyMath::Vector2 windowSize = {1280,720};

public:
	TransitionScene() = default;
	~TransitionScene() = default;

	//初期化
	void Initialize();

	/// <summary>
	/// 暗転時の挙動
	/// </summary>
	void DrawBlackOut();

	/// <summary>
	/// 明転時の挙動
	/// </summary>
	void DrawWhiteOut();

	/// <summary>
	/// ゲームオーバー時の挙動
	/// </summary>
	void GameOver();

public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	static TransitionScene* GetInstance();

private:
	//リセット処理
	void Reset();

private:
	Input* input = nullptr;

	//暗幕
	std::unique_ptr<Sprite> spriteBlack_;
	//縦のライン
	std::unique_ptr<Sprite> spriteLineLeft_;
	std::unique_ptr<Sprite> spriteLineRight_;
	//回す円のスプライト
	std::unique_ptr<Sprite> spriteCircle_;
	//背景の三角形
	std::unique_ptr<Sprite> spriteTriangleLeft_;
	std::unique_ptr<Sprite> spriteTriangleRight_;
	//ゲームオーバーの波線
	std::unique_ptr<Sprite> spriteWave_;

	TextureData texBlack_;
	TextureData texLine_;
	TextureData texCircle_;
	TextureData texTriangleBack_;
	TextureData texWave_;

	//テクスチャのカラー(主に透明度)
	MyMath::Vector4 color_;
	MyMath::Vector4 lineColor_;
	MyMath::Vector4 circleColor_;

	MyMath::Vector2 wavePos_;

	float circleRot = 0.0f;
	int8_t pad1[4 ];

	float adjustAlpha_ = 1.0f;
	int8_t pad2[4 ];

	float blackOutTimer_;
	int8_t pad3[ 4 ];

	bool alphaFlag = false;
	int8_t pad4[7 ];

	bool gameOverFlag = false;
	int8_t pad5[7 ];

	MyMath::Vector2 translation_;

private:
	enum class TRANSITION_COUNT
	{
		blackOut,
		whiteOut,
		Reset
	};

	enum COLOR
	{
		red = 1,
		green = 1,
		blue = 1
	};

	TRANSITION_COUNT count = TRANSITION_COUNT::blackOut;
	int8_t pad6[4 ];

private:
	//代入演算子削除
	TransitionScene& operator=(const TransitionScene&) = delete;
	//コピーコンストラクタ削除
	TransitionScene(const TransitionScene&) = delete;
};

