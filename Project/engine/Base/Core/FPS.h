#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <chrono>
#include <thread>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class FPS.h
 * @brief FPSを設定するクラス
 */

/// <summary>
/// FPS
/// </summary>
class FPS
{
public: // メンバ変数
	// FPS固定初期化
	void InitializeFixFps();
	// FPS固定更新
	void UpdateFixFPS();

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;
};
