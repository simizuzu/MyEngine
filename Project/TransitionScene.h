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
public:
	TransitionScene() = default;
	~TransitionScene() = default;

	//　初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了処理
	void Finalize();

public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	static TransitionScene* GetInstance();

private:
	/// <summary>
	/// 暗転時の挙動
	/// </summary>
	void BlackOut();

	/// <summary>
	/// 明転時の挙動
	/// </summary>
	void WhiteOut();

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

	TextureData texBlack_;
	TextureData texLine_;
	TextureData texCircle_;
	TextureData texTriangleBack_;

	//テクスチャのカラー(主に透明度)
	MyMath::Vector4 color_;
	MyMath::Vector4 lineColor_;
	MyMath::Vector4 circleColor_;

	float circleRot = 0.0f;
	int8_t pad7[4 ];

	float adjustAlpha_ = 1.0f;
	int8_t pad6[4 ];

	float blackOutTimer_;
	int8_t pad2[ 4 ];

	bool alphaFlag = false;
	int8_t pad3[7 ];

	MyMath::Vector2 translation_;

private:
	enum class TRANSITION_COUNT
	{
		blackOut,
		whiteOut,
		Reset
	};

	TRANSITION_COUNT count = TRANSITION_COUNT::blackOut;
	int8_t pad5[4 ];

private:
	//代入演算子削除
	TransitionScene& operator=(const TransitionScene&) = delete;
	//コピーコンストラクタ削除
	TransitionScene(const TransitionScene&) = delete;
};

