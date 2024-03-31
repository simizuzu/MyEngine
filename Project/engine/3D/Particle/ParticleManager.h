#pragma once

#include "BaseParticle.h"
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <forward_list>
#include <list>
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class ParticleManager.h
 * @brief パーティクルを生成するクラス
 */

/// <summary>
/// パーティクルマネージャ
/// </summary>
class ParticleManager
{
public:
	//コンストラクタ・デストラクタ
	ParticleManager() = default;
	~ParticleManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(const std::string& obj,Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="name">使用するパーティクルの番号 例：[1 = Explosion, 2 = Smoke]</param>
	/// <param name="amout">パーティクルの数</param>
	/// <param name="life">寿命</param>
	/// <param name="pos">座標</param>
	/// <param name="startScale">開始時のスケール</param>
	/// <param name="endScale">終了時のスケール</param>
	void Add(const std::string& name, uint16_t amout, uint8_t life, MyMath::Vector3 pos, float startScale, float endScale);

private:
	std::forward_list<std::unique_ptr<BaseParticle>> particles;
	std::string obj_;
	Camera* camera_ = nullptr;

private:

	//代入演算子削除
	ParticleManager& operator=(const ParticleManager&) = delete;
	//コピーコンストラクタ削除
	ParticleManager(const ParticleManager&) = delete;
};

