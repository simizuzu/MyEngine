#pragma once
#include "Sprite.h"
#include "TextureManager.h"
class PlayerDamage
{
public:
	PlayerDamage() = default;
	~PlayerDamage() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//ダメージ用全体スプライト(赤色)
	MyMath::Vector4 color;
	std::unique_ptr<Sprite> takenDamage;
	TextureData texTakenDamage;

private:
//コピーコンストラクタ・代入演算子削除
	PlayerDamage& operator=(const PlayerDamage&) = delete;
	PlayerDamage(const PlayerDamage&) = delete;
};

