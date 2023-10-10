#pragma once
#include "BaseScene.h"

#include "Sprite.h"
#include "TextureManager.h"

/// <summary>
/// シーン遷移用クラス
/// </summary>
class TransitionScene : public BaseScene
{
public:
	TransitionScene() = default;
	~TransitionScene() = default;

	//　初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了処理
	void Finalize() override;

private:
	std::unique_ptr<Sprite> spriteBlack_;
	TextureData texBlack_;

	float alpha_;
	int8_t pad1[4 ];

private:
	//代入演算子削除
	TransitionScene& operator=(const TransitionScene&) = delete;
	//コピーコンストラクタ削除
	TransitionScene(const TransitionScene&) = delete;
};

