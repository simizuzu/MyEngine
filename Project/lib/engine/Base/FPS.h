#pragma once
#include <chrono>
#include <thread>

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
